#include <string.h>


void *memcpy( void *dst, const void *src, size_t n )
{
    uint8_t *pdst = (uint8_t *)dst;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++)
    {
        pdst[i] = psrc[i];
    }

    return dst;
}


void *memcpy32( uint32_t *dst, const uint32_t *src, size_t count )
{
    for (size_t i=0; i<count; i++)
    {
        dst[i] = src[i];
    }

    return dst;
}


void *memset32( uint32_t *src, uint32_t value, size_t count )
{
    for (size_t i=0; i<count; i++)
    {
        src[i] = value;
    }

    return src;
}


