#pragma once
#include <stdint.h>

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
