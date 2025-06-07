#include <driver/interface.hpp>

#include <arch/io.hpp>
#include <filesystem/vfs.hpp>
#include <ipc/pipe.hpp>

#include <kernel/kvideo.hpp>
#include <kernel/log.hpp>
#include <kernel/event.hpp>
#include <kernel/ringbuffer.hpp>
#include <kthread.hpp>

#include <kmemxx.hpp>
#include <algorithm>


static vfsNode *msstate;
static knl::MsState mscurr;

struct MousePacket { uint8_t b0, b1, b2, b3; };
static knl::RingBuffer<MousePacket, 32> msbuf;


#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000100
#define PS2Rightbutton 0b00000010
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

void MouseWait()
{
    uint64_t timeout = 100000;
    while (timeout--)
    {
        if ((IO::inb(0x64) & 0b10) == 0)
        {
            return;
        }
    }
}

static void MouseWrite( uint8_t value )
{
    MouseWait();
    IO::outb(0x64, 0xD4);
    MouseWait();
    IO::outb(0x60, value);
}

uint8_t MouseRead()
{
    MouseWait();
    return IO::inb(0x60);
}


void ProcessMousePacket( const MousePacket &P )
{
    mscurr.l = (P.b0 & PS2Leftbutton);
    mscurr.m = (P.b0 & PS2Middlebutton);
    mscurr.r = (P.b0 & PS2Rightbutton);

    int xsign = (P.b0 & (1<<4)) ? -1 : +1;
    int ysign = (P.b0 & (1<<5)) ? -1 : +1;
    int xover = (P.b0 & (1<<6)) ? 255 : 0;
    int yover = (P.b0 & (1<<7)) ? 255 : 0;

    int dx = (xsign==1) ? P.b1 : 256-P.b1;
        dx = xsign * (dx - xover);
    int dy = (ysign==1) ? P.b2 : 256-P.b2;
        dy = ysign * (dy - yover);
    
    mscurr.x = std::clamp(mscurr.x + dx, 0, kvideo::W-1);
    mscurr.y = std::clamp(mscurr.y - dy, 0, kvideo::H-1);
}



static void mouse_init()
{
    // enabling the auxiliary device - mouse
    IO::outb(0x64, 0xA8);
    MouseWait();

    // tells the keyboard controller that we want to send a command to the mouse
    IO::outb(0x64, 0x20);
    MouseWait();

    uint8_t status = IO::inb(0x60);
    MouseWait();

    IO::outb(0x64, 0x60);
    MouseWait();

    // setting the correct bit is the "compaq" status byte
    IO::outb(0x60, status|0b10);

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
}


static void irq_handler( intframe_t* )
{
    static int count = 0;
    static MousePacket packet;
    uint8_t data = IO::inb(0x60);

    switch (count)
    {
        case 0:
            if ((data & 0b00001000) == 0) break;
            packet.b0 = data;
            count++;
            break;
        case 1:
            packet.b1 = data;
            count++;
            break;
        case 2:
            packet.b2 = data;
            count = 0;
            if (msbuf.size() < msbuf.capacity())
                msbuf.push_back(packet);
            break;
    }
}




static void msdev_main( void* )
{
    using namespace knl;
    mouse_init();
    msbuf.clear();

    while (true)
    {
        while (!msbuf.empty())
        {
            ProcessMousePacket(msbuf.front());
            msbuf.pop_front();
            vfs::write(msstate, &mscurr, 0, sizeof(mscurr));
        }

        kthread::yield();
    }
}


static void msdev_open()
{
    msstate = vfs::open("/dev/msstate");
}



static CharDevInterface msdev;

ModuleInterface *msdev_init( void* )
{
    // auto *msdev = (CharDevInterface*)std::malloc(sizeof(CharDevInterface));

    msdev.modtype  = ModuleType_Device;
    msdev.basetype = DeviceType_Mouse;
    msdev.main     = msdev_main;

    msdev.open     = msdev_open;
    msdev.close    = nullptr;
    msdev.read     = nullptr;
    msdev.write    = nullptr;
    msdev.irqno    = IrqNo_Mouse;
    msdev.irqfn    = irq_handler;

    kmemset<char>(msdev.signature, '\0', sizeof(msdev.signature));
    kmemcpy<char>(msdev.signature, "mouse", 5);

    return (ModuleInterface*)&msdev;
}


