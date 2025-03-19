#include <string.h>


void *memmove( void *dst, const void *src, size_t n )
{
    uint8_t *pdst = (uint8_t *)dst;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dst)
    {
        for (size_t i = 0; i < n; i++)
        {
            pdst[i] = psrc[i];
        }
    }

    else if (src < dst)
    {
        for (size_t i=n; i>0; i--)
        {
            pdst[i-1] = psrc[i-1];
        }
    }

    return dst;
}
