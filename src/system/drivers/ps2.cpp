#include "./ps2.hpp"
#include "serial.hpp"


void
ck::PS2::send( uint8_t cmd, uint8_t data )
{
    serial::outb(PORT_COMMAND, cmd);
    serial::outb(PORT_DATA, data);
}


uint8_t
ck::PS2::read()
{
    return serial::inb(PORT_DATA);
}


uint8_t
ck::PS2::status()
{
    return serial::inb(PORT_STATUS);
}


