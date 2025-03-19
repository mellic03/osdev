#include <string.h>


int strcmp( const char *s1, const char *s2 )
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }

    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}


int strncmp( const char *s1, const char *s2, size_t n )
{
    while (n && *s1 && ( *s1 == *s2 ))
    {
        ++s1;
        ++s2;
        --n;
    }

    if (n == 0)
    {
        return 0;
    }

    else
    {
        return *(unsigned char *)s1 - *(unsigned char *)s2;
    }
}

