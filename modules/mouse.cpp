#include <driver/interface.hpp>
#include <sym/sym.hpp>

#include <kernel/log.hpp>
#include <kernel/ioport.hpp>
#include <kernel/interrupt.hpp>
#include <kernel/kscancode.h>
#include <kmemxx.hpp>

struct MouseBitch
{
    int x, y;
    int l, m, r;
};

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
MouseBitch mbitch = {0, 0, 0, 0, 0};


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
    while (timeout--){
        if ((IO::inb(0x64) & 0b10) == 0){
            return;
        }
    }
}

void MouseWaitInput()
{
    uint64_t timeout = 100000;
    while (timeout--){
        if (IO::inb(0x64) & 0b1){
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
    MouseWaitInput();
    return IO::inb(0x60);
}

void ProcessMousePacket()
{
    if (!MousePacketReady)
    {
        return;
    }

    bool xNegative, yNegative, xOverflow, yOverflow;

    mbitch.l  = (MousePacket[0] & 0b01);
    mbitch.r = (MousePacket[0] & 0b10);

    xNegative = (MousePacket[0] & PS2XSign);
    yNegative = (MousePacket[0] & PS2YSign);
    xOverflow = (MousePacket[0] & PS2XOverflow);
    yOverflow = (MousePacket[0] & PS2YOverflow);

    if (!xNegative){
        mbitch.x += MousePacket[1];
        if (xOverflow){
            mbitch.x += 255;
        }
    } else
    {
        MousePacket[1] = 256 - MousePacket[1];
        mbitch.x -= MousePacket[1];
        if (xOverflow){
            mbitch.x -= 255;
        }
    }

    if (!yNegative){
        mbitch.y -= MousePacket[2];
        if (yOverflow){
            mbitch.y -= 255;
        }
    } else
    {
        MousePacket[2] = 256 - MousePacket[2];
        mbitch.y += MousePacket[2];
        if (yOverflow){
            mbitch.y += 255;
        }
    }

    mbitch.x = mbitch.x;
    mbitch.y = mbitch.y;
    MousePacketReady = false;
}



static void mouse_init()
{
    IO::outb(0x64, 0xA8); // enabling the auxiliary device - mouse

    MouseWait();
    IO::outb(0x64, 0x20); // tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
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
    std::printf("[mouse irq]\n");
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
        ProcessMousePacket();
        kthread::yield();
    }
}


static size_t driver_read( void *dstbuf, size_t nbytes )
{
    int *dst = (int*)dstbuf;
    // auto *src = (uint8_t*)(&mbitch);
    // size_t count = 0;

    dst[0] = mbitch.x;
    dst[1] = mbitch.y;

    // while ((count < nbytes) && (count < sizeof(MouseBitch)))
    // {
    //     *(dst++) = *(src++);
    //     count++;
    // }

    return 2;
}

static size_t driver_write( const void*, size_t )
{
    return 0;
}



static CharDevInterface mousedevice;


extern "C"
ModuleInterface *init( ksym::ksym_t *sym )
{
    ksym::loadsym(sym);
    kmemset(&mousedevice, 0, sizeof(mousedevice));

    mousedevice = {
        .modtype  = ModuleType_Device,
        .basetype = DeviceType_Char,
        .subtype  = DeviceType_Mouse,
        .main     = driver_main,

        .open     = nullptr,
        .close    = nullptr,
        .read     = driver_read,
        .write    = driver_write,
        .isrno    = IrqNo_Mouse,
        .isrfn    = irq_handler
    };

    auto &dev = mousedevice;
    kmemset<char>(dev.signature, '\0', sizeof(dev.signature));
    kmemcpy<char>(dev.signature, "Mouse", 5);

    return (ModuleInterface*)(&dev);
}


