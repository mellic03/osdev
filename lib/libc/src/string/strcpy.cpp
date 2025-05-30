#include <string.h>


char *strcpy( char *dst, const char *src )
{
    char *d = dst;
    while (*src)
        *(dst++) = *(src++);
    *dst = '\0';
    return d;
}


char *stpcpy( char *dst, const char *src )
{
    while (*src)
        *(dst++) = *(src++);
    *dst = '\0';
    return dst;
}


char *strncpy( char *dst, const char *src, size_t n )
{
    char *d = dst;
    size_t count = 0;

    while (*src && (count++ < n))
        *(dst++) = *(src++);
    *dst = '\0';

    return d;
}
