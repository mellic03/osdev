#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <kdef.h>
#include "./serial.h"


namespace idk::IO
{
    static constexpr uint16_t COM1 = 0x3F8;
    static constexpr uint16_t COM2 = 0x2F8;
    static constexpr uint16_t COM3 = 0x3E8;
    static constexpr uint16_t COM4 = 0x2E8;
    static constexpr uint16_t COM5 = 0x5F8;
    static constexpr uint16_t COM6 = 0x4F8;
    static constexpr uint16_t COM7 = 0x5E8;
    static constexpr uint16_t COM8 = 0x4E8;

    uint8_t  inb( uint16_t port );
    uint16_t inw( uint16_t port );
    uint32_t inl( uint16_t port );

    void outb(uint16_t port, uint8_t  data );
    void outw(uint16_t port, uint16_t data );
    void outl(uint16_t port, uint32_t data );

    void wait();
}


// namespace idk
// {
//     using in8  = IO::inb;
//     using in16 = IO::inw;
//     using in32 = IO::inl;

//     using out8  = IO::outb;
//     using out16 = IO::outw;
//     using out32 = IO::outl;
// }


namespace idk
{
    kret_t serial_init();
    void serialf( const char *fmt, ... );
}

