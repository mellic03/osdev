#include <string.h>


char *strchr( const char *str, int ch )
{
    while (*str)
    {
        if (*str == char(ch))
        {
            return (char*)(str);
        }

        str++;
    }

    return nullptr;
}

