#include <driver/pci.hpp>
#include <driver/svga.hpp>
#include "svga_reg.hpp"
#include "svga3d_reg.hpp"
#include "svga_overlay.hpp"

#include <arch/io.hpp>
#include <arch/mmio.hpp>
#include <cpu/cpu.hpp>

#include <kernel/boot_limine.hpp>
#include <kernel/log.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <kmemxx.hpp>


static bool       svga_enabled;
static SVGADevice svgadev;
// static SVGAScreenObject svga_displays[8];


void PCI_SetMemEnable( PCIAddress addr, bool enable )
{
   uint16_t command = PCI::read16(addr, 32);

   /* Mem space enable, IO space enable, bus mastering. */
   const uint16_t flags = 0x0007;

   if (enable) {
      command |= flags;
   } else {
      command &= ~flags;
   }

   PCI::write16(addr, 32, command);
}


// https://github.com/prepare/vmware-svga/blob/master/lib/refdriver/svga.c#L199

static bool SVGA_HasFIFOCap( uint32_t cap )
{
   return (svgadev.fifoMem[SVGA_FIFO_CAPABILITIES] & cap) != 0;
}

static bool SVGA_IsFIFORegValid( uint32_t reg )
{
   return svgadev.fifoMem[SVGA_FIFO_MIN] > (reg << 2);
}

static void SVGA_Enable()
{
    svgadev.fifoMem[SVGA_FIFO_MIN] = SVGA_FIFO_NUM_REGS * sizeof(uint32_t);
    svgadev.fifoMem[SVGA_FIFO_MAX] = svgadev.fifoSize;
    svgadev.fifoMem[SVGA_FIFO_NEXT_CMD] = svgadev.fifoMem[SVGA_FIFO_MIN];
    svgadev.fifoMem[SVGA_FIFO_STOP] = svgadev.fifoMem[SVGA_FIFO_MIN];

   /*
    * Prep work for 3D version negotiation. See SVGA3D_Init for
    * details, but we have to give the host our 3D protocol version
    * before enabling the FIFO.
    */

    if (SVGA_HasFIFOCap(SVGA_CAP_EXTENDED_FIFO)
        && SVGA_IsFIFORegValid(SVGA_FIFO_GUEST_3D_HWVERSION))
    {
        svgadev.fifoMem[SVGA_FIFO_GUEST_3D_HWVERSION] = SVGA3D_HWVERSION_CURRENT;
    }


   /*
    * Enable the SVGA device and FIFO.
    */

   SVGA::writeReg(SVGA_REG_ENABLE, true);
   SVGA::writeReg(SVGA_REG_CONFIG_DONE, true);

   /*
    * Now that the FIFO is initialized, we can do an IRQ sanity check.
    * This makes sure that the VM's chipset and our own IRQ code
    * works. Better to find out now if something's wrong, than to
    * deadlock later.
    *
    * This inserts a FIFO fence, does a legacy sync to drain the FIFO,
    * then ensures that we received all applicable interrupts.
    */

#ifndef REALLY_TINY
    // if (svgadev.capabilities & SVGA_CAP_IRQMASK)
    // {
    //     SVGA::writeReg(SVGA_REG_IRQMASK, SVGA_IRQFLAG_ANY_FENCE);
    //     SVGA_ClearIRQ();

    //     SVGA_InsertFence();

    //     SVGA::writeReg(SVGA_REG_SYNC, 1);
    //     while (SVGA_ReadReg(SVGA_REG_BUSY) != FALSE);

    //     SVGA::writeReg(SVGA_REG_IRQMASK, 0);

    //     /* Check whether the interrupt occurred without blocking. */
    //     if ((svgadev.irq.pending & SVGA_IRQFLAG_ANY_FENCE) == 0) {
    //         SVGA_Panic("SVGA IRQ appears to be present but broken.");
    //     }

    //     SVGA_WaitForIRQ();
    // }
#endif
}






void SVGA::init()
{
    auto *dev = PCI::findDevice(PCI_VENDOR_ID_VMWARE, PCI_DEVICE_ID_VMWARE_SVGA2);
    svga_enabled = false;

    if (!dev)
    {
        return;
    }


    PCI_SetMemEnable(dev->address, true);
    svgadev.ioBase   = PCI::getBARAddr(dev->address, 0);
    svgadev.fbMem    = (uint8_t*)((uint64_t)PCI::getBARAddr(dev->address, 1));
    svgadev.fifoMem  = (uint32_t*)((uint64_t)PCI::getBARAddr(dev->address, 2));

    // uintptr_t virt = PMM::allocPage() + PMM::hhdm;
    // VMM::mapPage((uintptr_t)(svgadev.fifoMem), virt);
    VMM::mapPage((uintptr_t)(svgadev.fifoMem), (uintptr_t)(svgadev.fifoMem));


    svgadev.deviceVersionId = SVGA_ID_2;
    do {
        SVGA::writeReg(SVGA_REG_ID, svgadev.deviceVersionId);
        if (SVGA::readReg(SVGA_REG_ID) == svgadev.deviceVersionId)
            break;
        svgadev.deviceVersionId--;
    } while (svgadev.deviceVersionId >= SVGA_ID_0);

    if (svgadev.deviceVersionId < SVGA_ID_0)
    {
        return;
    }

    if (svgadev.deviceVersionId >= SVGA_ID_1)
        svgadev.capabilities = SVGA::readReg(SVGA_REG_CAPABILITIES);
    else
        svgadev.capabilities = 0;

    svgadev.vramSize = SVGA::readReg(SVGA_REG_VRAM_SIZE);
    svgadev.fbSize   = SVGA::readReg(SVGA_REG_FB_SIZE);
    svgadev.fifoSize = SVGA::readReg(SVGA_REG_MEM_SIZE);

    syslog log("SVGA::init");
    log("ioBase:            0x%lx", svgadev.ioBase);
    log("fbMem:             0x%lx", svgadev.fbMem);
    log("fifoMem:           0x%lx", svgadev.fifoMem);
    log("fifoSize:          %lu",   svgadev.fifoSize);
    log("fbSize:            %lu",   svgadev.fbSize);
    log("vramSize:          %lu",   svgadev.vramSize);
    log("deviceVersionId:   %u",    svgadev.deviceVersionId);
    log("capabilities:      %lu",   svgadev.capabilities);
    log("");
    log("SVGA_CAP_RECT_COPY:        %u", bool(svgadev.capabilities & SVGA_CAP_RECT_COPY));
    log("SVGA_CAP_CURSOR:           %u", bool(svgadev.capabilities & SVGA_CAP_CURSOR));
    log("SVGA_CAP_CURSOR_BYPASS:    %u", bool(svgadev.capabilities & SVGA_CAP_CURSOR_BYPASS));
    log("SVGA_CAP_CURSOR_BYPASS_2:  %u", bool(svgadev.capabilities & SVGA_CAP_CURSOR_BYPASS_2));
    log("SVGA_CAP_8BIT_EMULATION:   %u", bool(svgadev.capabilities & SVGA_CAP_8BIT_EMULATION));
    log("SVGA_CAP_ALPHA_CURSOR:     %u", bool(svgadev.capabilities & SVGA_CAP_ALPHA_CURSOR));
    log("SVGA_CAP_3D:               %u", bool(svgadev.capabilities & SVGA_CAP_3D));
    log("SVGA_CAP_EXTENDED_FIFO:    %u", bool(svgadev.capabilities & SVGA_CAP_EXTENDED_FIFO));
    log("SVGA_CAP_MULTIMON:         %u", bool(svgadev.capabilities & SVGA_CAP_MULTIMON));
    log("SVGA_CAP_PITCHLOCK:        %u", bool(svgadev.capabilities & SVGA_CAP_PITCHLOCK));
    log("SVGA_CAP_IRQMASK:          %u", bool(svgadev.capabilities & SVGA_CAP_IRQMASK));
    log("SVGA_CAP_DISPLAY_TOPOLOGY: %u", bool(svgadev.capabilities & SVGA_CAP_DISPLAY_TOPOLOGY));
    log("SVGA_CAP_GMR:              %u", bool(svgadev.capabilities & SVGA_CAP_GMR));
    log("SVGA_CAP_TRACES:           %u", bool(svgadev.capabilities & SVGA_CAP_TRACES));
    log("SVGA_CAP_GMR2:             %u", bool(svgadev.capabilities & SVGA_CAP_GMR2));
    log("SVGA_CAP_SCREEN_OBJECT_2:  %u", bool(svgadev.capabilities & SVGA_CAP_SCREEN_OBJECT_2));
    log("");


    if (svgadev.capabilities & SVGA_CAP_IRQMASK)
    {
        SVGA::writeReg(SVGA_REG_IRQMASK, 0);
        IO::out32(svgadev.ioBase + SVGA_IRQSTATUS_PORT, 0xFF);
    }

    SVGA_Enable();

    auto *fb = limine_res.fb->framebuffers[0];
    SVGA::setMode(fb->width, fb->height, fb->bpp);
    // SVGA::setMode(1024, 768, 32);

    svga_enabled = true;
}


bool SVGA::enabled()
{
    return svga_enabled;
}


void SVGA::swapBuffers()
{
    // kmemset<uint128_t>(svga_frontbuffer, (uint128_t)0, svga_nbytes);
}



#include <kernel/kvideo.hpp>

void SVGA::setMode( uint32_t width, uint32_t height, uint32_t bpp )
{
    syslog log("SVGA::setMode");

    svgadev.width  = width;
    svgadev.height = height;
    svgadev.bpp    = bpp;

    SVGA::writeReg(SVGA_REG_WIDTH, width);
    SVGA::writeReg(SVGA_REG_HEIGHT, height);
    SVGA::writeReg(SVGA_REG_BITS_PER_PIXEL, bpp);
    SVGA::writeReg(SVGA_REG_ENABLE, true);
    svgadev.pitch = SVGA::readReg(SVGA_REG_BYTES_PER_LINE);

    log("w:     %u", svgadev.width);
    log("h:     %u", svgadev.height);
    log("bpp:   %u", svgadev.bpp);
    log("pitch: %u", svgadev.pitch);
}


uint32_t SVGA::readReg( uint32_t idx )
{
    IO::out32(svgadev.ioBase + SVGA_INDEX_PORT, idx);
    return IO::in32(svgadev.ioBase + SVGA_VALUE_PORT);
}


void SVGA::writeReg( uint32_t idx, uint32_t value )
{
   IO::out32(svgadev.ioBase + SVGA_INDEX_PORT, idx);
   IO::out32(svgadev.ioBase + SVGA_VALUE_PORT, value);
}














void SVGAFIFOFull()
{
    SVGA::writeReg(SVGA_REG_SYNC, 1);
    SVGA::readReg(SVGA_REG_BUSY);
}


#include <kpanic.hpp>

void *SVGA_FIFOReserve( uint32_t bytes )
{
    volatile uint32_t *fifo = svgadev.fifoMem;
    uint32_t max = fifo[SVGA_FIFO_MAX];
    uint32_t min = fifo[SVGA_FIFO_MIN];
    uint32_t nextCmd = fifo[SVGA_FIFO_NEXT_CMD];
    bool reserveable = SVGA_HasFIFOCap(SVGA_FIFO_CAP_RESERVE);

    /*
        * This example implementation uses only a statically allocated
        * buffer.  If you want to support arbitrarily large commands,
        * dynamically allocate a buffer if and only if it's necessary.
        */

    if (bytes > sizeof(svgadev.fifo.bounceBuffer) ||
        bytes > (max - min)) {
        kpanic("FIFO command too large");
    }

    if (bytes % sizeof(uint32_t)) {
        kpanic("FIFO command length not 32-bit aligned");
    }

    if (svgadev.fifo.reservedSize != 0) {
        kpanic("FIFOReserve before FIFOCommit");
    }

    svgadev.fifo.reservedSize = bytes;

    while (1) {
        uint32_t stop = fifo[SVGA_FIFO_STOP];
        bool reserveInPlace = false;
        bool needBounce = false;

        /*
        * Find a strategy for dealing with "bytes" of data:
        * - reserve in place, if there's room and the FIFO supports it
        * - reserve in bounce buffer, if there's room in FIFO but not
        *   contiguous or FIFO can't safely handle reservations
        * - otherwise, sync the FIFO and try again.
        */

        if (nextCmd >= stop) {
            /* There is no valid FIFO data between nextCmd and max */

            if (nextCmd + bytes < max ||
                (nextCmd + bytes == max && stop > min)) {
                /*
                * Fastest path 1: There is already enough contiguous space
                * between nextCmd and max (the end of the buffer).
                *
                * Note the edge case: If the "<" path succeeds, we can
                * quickly return without performing any other tests. If
                * we end up on the "==" path, we're writing exactly up to
                * the top of the FIFO and we still need to make sure that
                * there is at least one unused DWORD at the bottom, in
                * order to be sure we don't fill the FIFO entirely.
                *
                * If the "==" test succeeds, but stop <= min (the FIFO
                * would be completely full if we were to reserve this
                * much space) we'll end up hitting the FIFOFull path below.
                */
                reserveInPlace = true;
            } else if ((max - nextCmd) + (stop - min) <= bytes) {
                /*
                * We have to split the FIFO command into two pieces,
                * but there still isn't enough total free space in
                * the FIFO to store it.
                *
                * Note the "<=". We need to keep at least one DWORD
                * of the FIFO free at all times, or we won't be able
                * to tell the difference between full and empty.
                */
                SVGAFIFOFull();
            } else {
                /*
                * Data fits in FIFO but only if we split it.
                * Need to bounce to guarantee contiguous buffer.
                */
                needBounce = true;
            }

        } else {
            /* There is FIFO data between nextCmd and max */

            if (nextCmd + bytes < stop) {
                /*
                * Fastest path 2: There is already enough contiguous space
                * between nextCmd and stop.
                */
                reserveInPlace = true;
            } else {
                /*
                * There isn't enough room between nextCmd and stop.
                * The FIFO is too full to accept this command.
                */
                SVGAFIFOFull();
            }
        }

        /*
        * If we decided we can write directly to the FIFO, make sure
        * the VMX can safely support this.
        */
        if (reserveInPlace) {
            if (reserveable || bytes <= sizeof(uint32_t)) {
                svgadev.fifo.usingBounceBuffer = false;
                if (reserveable) {
                    fifo[SVGA_FIFO_RESERVED] = bytes;
                }
                return nextCmd + (uint8_t*)fifo;
            } else {
                /*
                * Need to bounce because we can't trust the VMX to safely
                * handle uncommitted data in FIFO.
                */
                needBounce = true;
            }
        }

        /*
        * If we reach here, either we found a full FIFO, called
        * SVGAFIFOFull to make more room, and want to try again, or we
        * decided to use a bounce buffer instead.
        */
        if (needBounce) {
            svgadev.fifo.usingBounceBuffer = true;
            return svgadev.fifo.bounceBuffer;
        }
    } /* while (1) */
}




void *SVGA_FIFOReserveEscape( uint32_t nsid, uint32_t bytes )
{
    uint32_t paddedBytes = (bytes + 3) & ~3UL;
    struct temp_type {
        uint32_t cmd;
        uint32_t nsid;
        uint32_t size;
    } __attribute__ ((__packed__));

    auto *header = (temp_type*)SVGA_FIFOReserve(paddedBytes + sizeof(temp_type));

    header->cmd = SVGA_CMD_ESCAPE;
    header->nsid = nsid;
    header->size = bytes;

    return header + 1;
}


#include <algorithm>

void SVGA_FIFOCommit( uint32_t bytes )
{
    volatile uint32_t *fifo = svgadev.fifoMem;
    uint32_t nextCmd = fifo[SVGA_FIFO_NEXT_CMD];
    uint32_t max = fifo[SVGA_FIFO_MAX];
    uint32_t min = fifo[SVGA_FIFO_MIN];
    bool reserveable = SVGA_HasFIFOCap(SVGA_FIFO_CAP_RESERVE);

    if (svgadev.fifo.reservedSize == 0) {
        return;
        // SVGA_Panic("FIFOCommit before FIFOReserve");
    }
    svgadev.fifo.reservedSize = 0;

    if (svgadev.fifo.usingBounceBuffer) {
        /*
        * Slow paths: copy out of a bounce buffer.
        */
        uint8_t *buffer = svgadev.fifo.bounceBuffer;

        if (reserveable) {
            /*
            * Slow path: bulk copy out of a bounce buffer in two chunks.
            *
            * Note that the second chunk may be zero-length if the reserved
            * size was large enough to wrap around but the commit size was
            * small enough that everything fit contiguously into the FIFO.
            *
            * Note also that we didn't need to tell the FIFO about the
            * reservation in the bounce buffer, but we do need to tell it
            * about the data we're bouncing from there into the FIFO.
            */

            uint32_t chunkSize = std::min(bytes, max - nextCmd);
            fifo[SVGA_FIFO_RESERVED] = bytes;
            kmemcpy(nextCmd + (uint8_t*) fifo, buffer, chunkSize);
            kmemcpy(min + (uint8_t*) fifo, buffer + chunkSize, bytes - chunkSize);

        } else {
            /*
            * Slowest path: copy one dword at a time, updating NEXT_CMD as
            * we go, so that we bound how much data the guest has written
            * and the host doesn't know to checkpoint.
            */

            // uint8_t *reeee = buffer;
            uint32_t *dword = (uint32_t*)buffer;

            while (bytes > 0) {

                // for (int i=0; i<4; i++)
                //     fifo[nextCmd / sizeof(uint32_t) + i] = *(reeee + i);
                // reeee += sizeof(uint32_t);

                // fifo[nextCmd / sizeof(uint32_t)] = *((uint32_t*)reeee); reeee += sizeof(uint32_t);
                fifo[nextCmd / sizeof(uint32_t)] = *(dword++);
                nextCmd += sizeof(uint32_t);
                if (nextCmd == max)
                    nextCmd = min;
                fifo[SVGA_FIFO_NEXT_CMD] = nextCmd;
                bytes -= sizeof(uint32_t);
            }
        }
    }

    /*
        * Atomically update NEXT_CMD, if we didn't already
        */
    if (!svgadev.fifo.usingBounceBuffer || reserveable) {
        nextCmd += bytes;
        if (nextCmd >= max) {
            nextCmd -= max - min;
        }
        fifo[SVGA_FIFO_NEXT_CMD] = nextCmd;
    }

    /*
        * Clear the reservation in the FIFO.
        */
    if (reserveable) {
        fifo[SVGA_FIFO_RESERVED] = 0;
    }
}


void SVGA_FIFOCommitAll()
{
   SVGA_FIFOCommit(svgadev.fifo.reservedSize);
}


#define SVGA_ESCAPE_NSID_VMWARE 0x00000000

void SVGA::videoFlush( uint32_t streamId )
{
   SVGAEscapeVideoFlush *cmd;

   cmd = (SVGAEscapeVideoFlush*)SVGA_FIFOReserveEscape(SVGA_ESCAPE_NSID_VMWARE, sizeof(*cmd));
   cmd->cmdType = SVGA_ESCAPE_VMWARE_VIDEO_FLUSH;
   cmd->streamId = streamId;
   SVGA_FIFOCommitAll();
}

void *SVGA_FIFOReserveCmd( uint32_t type, uint32_t nbytes )
{
   uint32_t *cmd = (uint32_t*)SVGA_FIFOReserve(sizeof(type) + nbytes);
   cmd[0] = type;
   return cmd + 1;
}

void SVGA::update( uint32_t x, uint32_t y, uint32_t w, uint32_t h )
{
    // uint32_t *cmd = (uint32_t*)SVGA_FIFOReserve(sizeof(uint32_t) + sizeof(SVGAFifoCmdUpdate));
    // cmd[0] = SVGA_CMD_UPDATE;
    // cmd[1] = x;
    // cmd[2] = y;
    // cmd[3] = w;
    // cmd[4] = h;
    auto *cmd = (SVGAFifoCmdUpdate*)SVGA_FIFOReserveCmd(SVGA_CMD_UPDATE, sizeof(SVGAFifoCmdUpdate));
    cmd->x = x;
    cmd->y = y;
    cmd->width = w;
    cmd->height = h;
    SVGA_FIFOCommitAll();

}






class VideoInterfaceSVGA: public knl::VideoInterface
{
private:
    uint8_t *m_back;

public:
    virtual bool available() final
    {
        auto *dev = PCI::findDevice(PCI_VENDOR_ID_VMWARE, PCI_DEVICE_ID_VMWARE_SVGA2);
        return (dev != nullptr);
    }

    virtual void init() final
    {
        SVGA::init();
    
        // kvideo2::setMode(fb->width, fb->height, fb->bpp);

        using namespace VMM;
        size_t nbytes = svgadev.pitch * svgadev.height;
        nbytes = std::min((uint32_t)nbytes, svgadev.fbSize);

        syslog log("VideoInterfaceSVGA::init");
        log("nbytes: %lu", nbytes);
        m_back = (uint8_t*)VMM::alloc(nbytes, PAGE_PRESENT|PAGE_WRITE);
        // m_back = (uint8_t*)VMM::alloc(nbytes, PAGE_PRESENT|PAGE_WRITE|PAGE_PWT|PAGE_PCD);
    }

    virtual void setMode( uint32_t w, uint32_t h, uint32_t bpp ) final
    {
        SVGA::setMode(w, h, bpp);

        m_mode = {
            .w     = svgadev.width,
            .h     = svgadev.height,
            .bpp   = svgadev.bpp,
            .pitch = svgadev.pitch,
        };
    }

    virtual void flush( uint32_t x, uint32_t y, uint32_t w, uint32_t h ) final
    {
        size_t nbytes = svgadev.pitch * svgadev.height;
               nbytes = std::min((uint32_t)nbytes, svgadev.fbSize);

        syslog log("VideoInterfaceSVGA::flush");
        log("nbytes: %lu", nbytes);
        // CPU::movs64(svgadev.fbMem + PMM::hhdm, m_back, nbytes/8);
        // CPU::stos64(m_back, 0, nbytes/8);
        kmemcpy<uint128_t>(svgadev.fbMem + PMM::hhdm, m_back, nbytes);
        kmemset<uint128_t>(m_back, 0, nbytes);
        SVGA::update(x, y, w, h);
    }

    virtual uint8_t *getFb() final
    {
        return m_back;
    }

    virtual void drawRect( int x, int y, int w, int h, kvideo2::ColorRGBA8 color ) final
    {
        uint32_t pitch = Pitch();
        uint32_t bypp  = BYPP();
        uint8_t *dst   = (uint8_t*)m_back;

        for (int i=y; i<y+h; i++)
        {
            // uint32_t idx = pitch*i + bypp*x;
            // kmemset<uint32_t>(m_back + idx, C.dword, bypp*w);
            for (int j=x; j<x+w; j++)
            {
                uint32_t idx = pitch*i + bypp*j;
                dst[idx+0] = color.rgba[0];
                dst[idx+1] = color.rgba[1];
                dst[idx+2] = color.rgba[2];
                dst[idx+3] = color.rgba[3];
            }
        }
    }

};



knl::VideoInterface *SVGA::getInterface()
{
    static VideoInterfaceSVGA I;
    return (knl::VideoInterface*)(&I);
}

