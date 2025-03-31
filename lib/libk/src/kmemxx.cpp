#include <kmemxx.h>

void *memcpy8( uint8_t *dst, const uint8_t *src, size_t n )
{
    while (n--) { *(dst++) = *(src++); }
    return dst-n;
}

void *memcpy16( uint16_t *dst, const uint16_t *src, size_t n )
{
    while (n--) { *(dst++) = *(src++); }
    return dst-n;
}

void *memcpy32( uint32_t *dst, const uint32_t *src, size_t n )
{
    while (n--) { *(dst++) = *(src++); }
    return dst-n;
}

void *memcpy64( uint64_t *dst, const uint64_t *src, size_t n )
{
    while (n--) { *(dst++) = *(src++); }
    return dst-n;
}

void *memcpy128( uint128_t *dst, const uint128_t *src, size_t n )
{
    while (n--) { *(dst++) = *(src++); }
    return dst-n;
}

// void *kmemcpy( void *d, const void *s, size_t nbytes )
// {
//     auto dst = (uintptr_t)d;
//     auto src = (uintptr_t)s;

//     size_t count = nbytes/sizeof(uint128_t);
//     size_t rem   = nbytes % sizeof(uint128_t);
//     memcpy128((uint128_t*)dst, (uint128_t*)src, count);
//     dst += count*sizeof(uint128_t);
//     src += count*sizeof(uint128_t);
//     memcpy8((uint8_t*)dst, (uint8_t*)src, rem);

//     return d;
// }

// DO_THING(8)
// DO_THING(16)
// DO_THING(32)
// DO_THING(64)
// DO_THING(128)


// void *memcpy16  ( uint16_t*,  const uint16_t*,  size_t n );
// void *memcpy32  ( uint32_t*,  const uint32_t*,  size_t n );
// void *memcpy64  ( uint64_t*,  const uint64_t*,  size_t n );
// void *memcpy128 ( uint128_t*, const uint128_t*, size_t n );


