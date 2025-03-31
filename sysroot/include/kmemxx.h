#pragma once

#include <AAA.h>


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


#include <ZZZ.h>


#ifdef __cplusplus

    template <typename T>
    inline constexpr
    void *kmemcpy( void *dst, const void *src, size_t nbytes )
    {
        auto  n = nbytes / sizeof(T);
        auto *d = (T*)dst;
        auto *s = (T*)src;
        for (size_t i=0; i<n; i++)
            *(d++) = *(s++);
        return dst;
    }

    template <typename T>
    inline constexpr
    void *kmemset( void *dst, T value, size_t nbytes )
    {
        auto  n = nbytes / sizeof(T);
        auto *d = (T*)dst;
        for (size_t i=0; i<n; i++)
            *(d++) = value;
        return dst;
    }

#endif

