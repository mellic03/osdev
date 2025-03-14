#include "./kb-ps2.hpp"
#include "./serial.hpp"
#include "../io.hpp"


void
idk::PS2::send( uint8_t cmd, uint8_t data )
{
    IO::outb(PORT_COMMAND, cmd);
    IO::outb(PORT_DATA, data);
}


uint8_t
idk::PS2::read()
{
    return IO::inb(PORT_DATA);
}


uint8_t
idk::PS2::status()
{
    return IO::inb(PORT_STATUS);
}


char
idk::PS2::kb_poll()
{
    uint8_t opcode = IO::inb(PORT_DATA);

    switch (opcode)
    {
        default:   return '\0';
        case 0x11: return 'w';
        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
    }
}


void
mouse_wait(uint8_t a_type)
{
    uint32_t timeout = 100000;
    uint8_t bit, desired;

    if (a_type == 0)
    {
        bit = 1;
        desired = 1;
    }

    else
    {
        bit = 2;
        desired = 0;
    }


    while (timeout--)
    {
        if ((idk::IO::inb(0x64) & bit) == desired)
        {
            return;
        }
    }

}


void
mouse_write( uint8_t a_write )
{
    // Tell mouse we are sending a command
    mouse_wait(1);
    idk::IO::outb(0x64, 0xD4);
    mouse_wait(1);
    //Finally write
    idk::IO::outb(0x60, a_write);
}


uint8_t
mouse_read()
{
    mouse_wait(0);
    return idk::IO::inb(0x60);
}



void mouse_enable()
{
    uint8_t status;  //unsigned char
    //Enable the auxiliary mouse device
    mouse_wait(1);
    idk::IO::outb(0x64, 0xA8);

    //Enable the interrupts
    mouse_wait(1);
    idk::IO::outb(0x64, 0x20);
    mouse_wait(0);
    status = (idk::IO::inb(0x60) | 2);
    mouse_wait(1);
    idk::IO::outb(0x64, 0x60);
    mouse_wait(1);
    idk::IO::outb(0x60, status);

    // Tell the mouse to use default settings
    mouse_write(0xF6);
    mouse_read();  //Acknowledge

    // Enable the mouse
    mouse_write(0xF4);
    mouse_read();  //Acknowledge

}
