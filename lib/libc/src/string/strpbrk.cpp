#include <string.h>


char *strpbrk( const char *str, const char *brk )
{
    while (*str)
    {
        if (strchr(brk, *str))
        {
            return (char*)str;
        }

        str++;
    }

    return nullptr;
}
