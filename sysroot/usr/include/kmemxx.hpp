#pragma once

#include <kdef.h>


void *memcpy8   ( uint8_t*,   const uint8_t*,   size_t n );
void *memcpy16  ( uint16_t*,  const uint16_t*,  size_t n );
void *memcpy32  ( uint32_t*,  const uint32_t*,  size_t n );
void *memcpy64  ( uint64_t*,  const uint64_t*,  size_t n );
void *memcpy128 ( uint128_t*, const uint128_t*, size_t n );
// void *kmemcpy   ( void*, const void*, size_t n );

// void *memset8   ( uint8_t*,   const uint8_t*,   size_t n );
// void *memset16  ( uint16_t*,  const uint16_t*,  size_t n );
// void *memset32  ( uint32_t*,  const uint32_t*,  size_t n );
// void *memset64  ( uint64_t*,  const uint64_t*,  size_t n );
// void *memset128 ( uint128_t*, const uint128_t*, size_t n );


// template <typename T=uint8_t>
// inline constexpr
// void *kmemcpy( void *dst, const void *src, size_t nbytes )
// {
//     auto *dstptr = reinterpret_cast<T*>(dst);
//     auto *dstend = reinterpret_cast<T*>((uintptr_t)dst + nbytes);
//     auto *srcptr = reinterpret_cast<const T*>(src);

//     while (dstptr < dstend)
//     {
//         *(dstptr++) = *(srcptr++);
//     }

//     return dst;
// }

template <typename T=uint8_t>
inline constexpr
void *kmemcpy( void *dst, const void *src, size_t nbytes )
{
    auto  *dstptr = (T*)dst;
    auto  *srcptr = (const T*)src;
    size_t count  = nbytes / sizeof(T);

    for (size_t i=0; i<count; i++)
    {
        dstptr[i] = srcptr[i];
    }

    return dst;
}


template <typename T=uint8_t>
inline constexpr
void *kmemset( void *dst, T value, size_t nbytes )
{
    auto  *dstptr = (T*)dst;
    size_t count  = nbytes / sizeof(T);

    for (size_t i=0; i<count; i++)
    {
        dstptr[i] = value;
    }

    return dst;
}


