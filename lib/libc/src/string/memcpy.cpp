#include <string.h>
#include <immintrin.h>


void memcpy_sse32( void *dst_ptr, const void *src_ptr, uint32_t count )
{
    uint32_t *dst = (uint32_t *)dst_ptr;
    const uint32_t *src = (const uint32_t *)src_ptr;

    __m128i reg;

    for (uint32_t i=0; i<count; i+=4)
    {
        reg = _mm_loadu_si128((__m128i *)(src+i));
        _mm_storeu_si128((__m128i *)(dst+i), reg);
    }

    uint32_t rem = count % 4;
    for (uint32_t i=count-rem; i<count; i++)
    {
        dst[i] = src[i];
    }
}


void *memcpy_u8( void *dst, const void *src, size_t n )
{
    uint8_t *pdst = (uint8_t *)dst;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i=0; i<n; i++)
    {
        pdst[i] = psrc[i];
    }

    return (void*)(pdst + n);
}



void *memcpy( void *dst, const void *src, size_t n )
{
    // size_t count = n / sizeof(uint32_t);
    // size_t rem   = n % sizeof(uint32_t);
    // memcpy_sse32(dst, src, count);

    // uint8_t *pdst = (uint8_t *)dst;
    // const uint8_t *psrc = (const uint8_t *)src;

    // size_t offset = count * sizeof(uint32_t);
    // memcpy_u8(pdst+offset, psrc+offset, 4*rem);
    memcpy_u8(dst, src, n);

    return dst;
}

