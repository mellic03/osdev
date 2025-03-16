#pragma once

#include <stdint.h>
#include <stddef.h>

namespace idk
{
    template <typename T>
    constexpr T bit_set( T b, T n )
    {
        return b | (1<<n);
    }

    template <typename T>
    constexpr T bit_unset( T b, T n )
    {
        return b & ~(1<<n);
    }

    template <typename T>
    constexpr T bit_toggle( T b, T n )
    {
        return b ^ (1<<n);
    }


    template <typename T>
    constexpr T *ptr_align( T *ptr, size_t alignment )
    {
        size_t p = reinterpret_cast<size_t>(ptr);
        return reinterpret_cast<T*>((p + alignment - 1) & ~(alignment - 1));
    }


    inline static size_t
    nearest_pow2( size_t v )
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        v++;
    
        return v;
    }

    inline static uint32_t
    bit_index( uint32_t v )
    {
        /*
            https://stackoverflow.com/questions/71539154/convert-power-of-2-bitmask-into-corresponding-index
        */
        v -= 1;
        v = v - ((v >> 1) & 0x55555555);
        v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
        return ((((v+ (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24);
    }


}