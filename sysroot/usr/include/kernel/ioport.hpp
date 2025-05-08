#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <kdef.h>

// #ifdef __cplusplus
// extern "C" {
// #endif


namespace IO
{
    static constexpr uint16_t COM1 = 0x3F8;
    static constexpr uint16_t COM2 = 0x2F8;
    static constexpr uint16_t COM3 = 0x3E8;
    static constexpr uint16_t COM4 = 0x2E8;
    static constexpr uint16_t COM5 = 0x5F8;
    static constexpr uint16_t COM6 = 0x4F8;
    static constexpr uint16_t COM7 = 0x5E8;
    static constexpr uint16_t COM8 = 0x4E8;

    inline uint8_t inb( uint16_t port )
    {
        uint8_t value;
        __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
        return value;
    }

    inline uint16_t inw( uint16_t port )
    {
        uint16_t value;
        __asm__ volatile("inw %1, %0" : "=a"(value) : "Nd"(port));
        return value;
    }

    inline uint32_t inl( uint16_t port )
    {
        uint32_t value;
        __asm__ volatile ("inl %w1,%0":"=a" (value):"Nd" (port));
        return value;
    }

    inline void outb( uint16_t port, uint8_t value )
    {
        __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
    }

    inline void outw( uint16_t port, uint16_t value )
    {
        __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
    }

    inline void outl( uint16_t port, uint32_t value )
    {
        __asm__ volatile("outl %0, %w1" : : "a"(value), "Nd"(port));
    }

    inline void wait()
    {
        IO::outb(0x80, 0);
    }

    const auto in8  = IO::inb;
    const auto in16 = IO::inw;
    const auto in32 = IO::inl;

    const auto out8  = IO::outb;
    const auto out16 = IO::outw;
    const auto out32 = IO::outl;
}


namespace MMIO
{
    inline static void out8( void *p, uint8_t data )
    {
        *(volatile uint8_t*)(p) = data;
    }

    inline static uint8_t in8( void *p )
    {
        return *(volatile uint8_t*)(p);
    }

    inline static void out16( void *p, uint16_t data )
    {
        *(volatile uint16_t*)(p) = data;
    }

    inline static uint16_t in16( void *p )
    {
        return *(volatile uint16_t*)(p);
    }

    inline static void out32( void *p, uint32_t data )
    {
        *(volatile uint32_t*)(p) = data;
    }

    inline static uint32_t in32( void *p )
    {
        return *(volatile uint32_t*)(p);
    }
}


// #ifdef __cplusplus
// }
// #endif



// #ifndef __cplusplus
//     inline uint8_t IO_inb( uint16_t port )
//     {
//         uint8_t value;
//         __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
//         return value;
//     }

//     inline void IO_outb( uint16_t port, uint8_t value )
//     {
//         __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
//     }
// #endif

