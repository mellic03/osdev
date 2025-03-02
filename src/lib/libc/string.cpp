#include <string.h>


size_t strlen( const char *str ) 
{
	const char *base = str;
	const char *end  = str;

	while (*end)
	{
		end++;
	}

	return (size_t)(end - base - 1);
}


int strcmp( const char *lhs, const char *rhs )
{
    while (*lhs && *lhs == *rhs)
    {
        lhs++;
        rhs++;
    }

    return (*lhs > *rhs) - (*rhs > *lhs);
}




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



void *memset( void *s, uint8_t c, size_t n )
{
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++)
    {
        p[i] = (uint8_t)c;
    }

    return s;
}


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


int memcmp( const void *s1, const void *s2, size_t n )
{
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i=0; i<n; i++)
    {
        if (p1[i] != p2[i])
        {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}



