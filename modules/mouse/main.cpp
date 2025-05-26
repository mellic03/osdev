#include <driver/interface.hpp>
#include <sym/sym.hpp>

#include <kernel/log.hpp>
#include <arch/io.hpp>
#include <kernel/event.hpp>
#include <kernel/interrupt.hpp>
#include <kernel/kscancode.h>
#include <kmemxx.hpp>
#include <algorithm>


static knl::MsState msdata;
static uint8_t MouseCycle = 0;
static uint8_t MousePacket[4];
static bool MousePacketReady = false;


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

bool ProcessMousePacket()
{
    if (!MousePacketReady)
    {
        return false;
    }

    bool xNegative, yNegative, xOverflow, yOverflow;

    msdata.prevDown   = msdata.currDown;
    msdata.currDown.l = (MousePacket[0] & PS2Leftbutton);
    msdata.currDown.m = (MousePacket[0] & PS2Middlebutton);
    msdata.currDown.r = (MousePacket[0] & PS2Rightbutton);

    // if (msdata.prevDown.l != msdata.currDown.l)
    // {
    //     knl::MsEvent event = { knl::MsBtn_Left, knl::BtnAction_Up };
    //     knl::writeMsEvent(event);
    // }

    xNegative = (MousePacket[0] & PS2XSign);
    yNegative = (MousePacket[0] & PS2YSign);
    xOverflow = (MousePacket[0] & PS2XOverflow);
    yOverflow = (MousePacket[0] & PS2YOverflow);

    if (!xNegative){
        msdata.x += MousePacket[1];
        if (xOverflow){
            msdata.x += 255;
        }
    } else
    {
        MousePacket[1] = 256 - MousePacket[1];
        msdata.x -= MousePacket[1];
        if (xOverflow){
            msdata.x -= 255;
        }
    }

    if (!yNegative){
        msdata.y -= MousePacket[2];
        if (yOverflow){
            msdata.y -= 255;
        }
    } else
    {
        MousePacket[2] = 256 - MousePacket[2];
        msdata.y += MousePacket[2];
        if (yOverflow){
            msdata.y += 255;
        }
    }

    msdata.x = std::clamp(msdata.x, 0, kvideo::W-1);
    msdata.y = std::clamp(msdata.y, 0, kvideo::H-1);

    MousePacketReady = false;
    return true;
}



static void mouse_init()
{
    IO::outb(0x64, 0xA8); // enabling the auxiliary device - mouse

    MouseWait();
    IO::outb(0x64, 0x20); // tells the keyboard controller that we want to send a command to the mouse
    MouseWait();
    uint8_t status = IO::inb(0x60);
    status |= 0b10;
    MouseWait();
    IO::outb(0x64, 0x60);
    MouseWait();
    IO::outb(0x60, status); // setting the correct bit is the "compaq" status byte

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
}


static void irq_handler( intframe_t* )
{
    uint8_t data = IO::inb(0x60);
    switch(MouseCycle){
        case 0:
            if (MousePacketReady) break;
            if ((data & 0b00001000) == 0) break;
            MousePacket[0] = data;
            MouseCycle++;
            break;
        case 1:
            if (MousePacketReady) break;
            MousePacket[1] = data;
            MouseCycle++;
            break;
        case 2:
            if (MousePacketReady) break;
            MousePacket[2] = data;
            MousePacketReady = true;
            MouseCycle = 0;
            break;
    }
}




static void driver_main( void* )
{
    mouse_init();

    while (true)
    {
        if (ProcessMousePacket())
        {
            kthread::yield();
        }
    }
}


static size_t driver_read( void *dst, size_t max_nbytes )
{
    if (sizeof(msdata) > max_nbytes)
        return 0;
    *(knl::MsState*)dst = msdata;
    return sizeof(msdata);
}



extern "C"
ModuleInterface *init( ksym::ksym_t *sym )
{
    ksym::loadsym(sym);

    auto *msdev = (CharDevInterface*)std::malloc(sizeof(CharDevInterface));

    *msdev = {
        .modtype  = ModuleType_Device,
        .basetype = DeviceType_Mouse,
        .main     = driver_main,

        .open     = nullptr,
        .close    = nullptr,
        .read     = driver_read,
        .write    = nullptr,
        .irqno    = IrqNo_Mouse,
        .irqfn    = irq_handler
    };

    kmemset<char>(msdev->signature, '\0', sizeof(msdev->signature));
    kmemcpy<char>(msdev->signature, "mouse", 5);

    return (ModuleInterface*)msdev;
}


