#pragma once

#include <stdint.h>
#include <stddef.h>

namespace ck
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
}