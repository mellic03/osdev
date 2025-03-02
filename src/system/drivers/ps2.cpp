#include "./ps2.hpp"
#include "./serial.hpp"
#include "../io.hpp"


void
ck::PS2::send( uint8_t cmd, uint8_t data )
{
    IO::outb(PORT_COMMAND, cmd);
    IO::outb(PORT_DATA, data);
}


uint8_t
ck::PS2::read()
{
    return IO::inb(PORT_DATA);
}


uint8_t
ck::PS2::status()
{
    return IO::inb(PORT_STATUS);
}


