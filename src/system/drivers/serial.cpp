#include "serial.hpp"

int
ck::serial::init()
{
    static int first = true;

    if (first == false)
    {
        return 1;
    }

    first = false;

    serial::outb(COM1+1, 0x00);    // Disable all interrupts
    serial::outb(COM1+3, 0x80);    // Enable DLAB (set baud rate divisor)
    serial::outb(COM1+0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    serial::outb(COM1+1, 0x00);    //                  (hi byte)
    serial::outb(COM1+3, 0x03);    // 8 bits, no parity, one stop bit
    serial::outb(COM1+2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    serial::outb(COM1+4, 0x0B);    // IRQs enabled, RTS/DSR set
    serial::outb(COM1+4, 0x1E);    // Set in loopback mode, test the serial chip
    serial::outb(COM1+0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
    // Check if serial is faulty (i.e: not same byte as sent)
    if (serial::inb(COM1 + 0) != 0xAE)
    {
        return 0;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    serial::outb(COM1 + 4, 0x0F);

    serial::writestr("[ck::serial::init]\n");

    return 1;
}


int
ck::serial::writestr( const char *str )
{
    init();

    while (*str)
    {
        serial::outb(COM1, *str);
        str++;
    }

    return 0;
}



uint8_t
ck::serial::inb( uint16_t port )
{
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory" );
    return ret;
}


void
ck::serial::outb( uint16_t port, uint8_t value )
{
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port) :"memory");
}



uint16_t
ck::serial::inw( uint16_t port )
{
    uint16_t value;
    asm volatile("in" "w" " %w1, %" "w" "0" : "=a"(value) : "Nd"(port));
    return value;
}


void
ck::serial::outw( uint16_t port, uint16_t value )
{
    asm volatile("out" "w" " %" "w" "0, %w1" : : "a"(value), "Nd"(port));
}
