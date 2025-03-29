#include <string.h>
#include <immintrin.h>



void memset_sse32( void *ptr, uint32_t value, uint32_t count )
{
    uint32_t *dst = (uint32_t *)(ptr);
    __m128i  reg = _mm_set1_epi32(value);
    uint32_t rem = count % 4;

    for (uint32_t i=0; i<count; i+=4)
    {
        _mm_storeu_si128((__m128i *)(&dst[i]), reg);
    }

    for (uint32_t i=count-rem; i<count; i++)
    {
        dst[i] = value;
    }
}


void *memset_u8( uint8_t *p, uint8_t c, size_t n )
{
    for (size_t i = 0; i < n; i++)
    {
        p[i] = (uint8_t)c;
    }

    return p;
}


void *memset( void *s, uint8_t c, size_t n )
{
    // uint32_t v = uint32_t(c);
    // uint32_t value = (v<<24) + (v<<16) + (v<<8) + (v<<0);

    // size_t count = n / sizeof(uint32_t);
    // size_t rem   = n % sizeof(uint32_t);
    // memset_sse32(s, value, count);


    // size_t offset = count * sizeof(uint32_t);
    // memset_u8((uint8_t *)s + offset, c, 4*rem);

    // return s;

    uint8_t *p = (uint8_t*)s;

    for (size_t i=0; i<n; i++)
    {
        p[i] = (uint8_t)c;
    }

    return p;
}

