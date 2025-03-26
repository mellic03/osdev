#include <string.h>


char *strcpy( char *dst, const char *src )
{
    while (*src)
    {
        *(dst++) = *(src++);
    }
    *dst = '\0';

    return dst;
}


char *strncpy( char *dst, const char *src, size_t n )
{
    size_t count = 0;

    while (*src && count < n)
    {
        *(dst++) = *(src++);
        count++;
    }
    *dst = '\0';

    return dst;
}
