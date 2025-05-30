#include <string.h>


char *strstr( char const *str, char const *substr )
{
    if (strlen(substr) == 0)
        return (char*)str;

    while (*str && *substr)
    {
        if (strcmp(str, substr) == 0)
            return (char*)str;
        str++;
    }

    return nullptr;
}
