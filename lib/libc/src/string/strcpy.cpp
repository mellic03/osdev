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
