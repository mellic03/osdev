#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <kdef.h>


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

    uint8_t  inb( uint16_t port );
    uint16_t inw( uint16_t port );
    uint32_t inl( uint16_t port );
    void outb( uint16_t port, uint8_t  data );
    void outw( uint16_t port, uint16_t data );
    void outl( uint16_t port, uint32_t data );

    uint8_t  in8 ( uint16_t port );
    uint16_t in16( uint16_t port );
    uint32_t in32( uint16_t port );
    void out8 ( uint16_t port, uint8_t  data );
    void out16( uint16_t port, uint16_t data );
    void out32( uint16_t port, uint32_t data );

    void wait();


    inline static void mmOut8( void *p, uint8_t data )
    {
        *(volatile uint8_t*)(p) = data;
    }

    inline static uint8_t mmIn8( void *p )
    {
        return *(volatile uint8_t*)(p);
    }

    inline static void mmOut16( void *p, uint16_t data )
    {
        *(volatile uint16_t*)(p) = data;
    }

    inline static uint16_t mmIn16( void *p )
    {
        return *(volatile uint16_t*)(p);
    }

    inline static void mmOut32( void *p, uint32_t data )
    {
        *(volatile uint32_t*)(p) = data;
    }

    inline static uint32_t mmIn32( void *p )
    {
        return *(volatile uint32_t*)(p);
    }
}
