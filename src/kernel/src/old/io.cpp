#include "./io.hpp"


uint8_t idk::IO::inb( uint16_t port )
{
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint16_t idk::IO::inw( uint16_t port )
{
    uint16_t ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint32_t idk::IO::inl( uint16_t port )
{
    uint32_t ret;
    __asm__ __volatile__ ("inl %w1,%0":"=a" (ret):"Nd" (port));
    return ret;
}


void idk::IO::outb( uint16_t port, uint8_t value )
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}


void idk::IO::outw( uint16_t port, uint16_t value )
{
    __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}


void idk::IO::outl( uint16_t port, uint32_t value )
{
    __asm__ volatile("outl %0, %w1" : : "a"(value), "Nd"(port));
}


void idk::IO::wait()
{
    IO::outb(0x80, 0x00);
}