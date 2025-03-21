#include <string.h>


size_t strspn( const char *str, const char *chars )
{
    size_t i;
    char ch[256] = {0};
  
    for (i=0; i<strlen(chars); i++)
    {
        ch[int(chars[i])] = 1;
    }
  
    for (i=0; i<strlen(str); i++)
    {
        if (ch[int(str[i])] == 0)
        {
            break;
        }
    }
  
    return i;
}


size_t strcspn( const char *str, const char *bad )
{
    static unsigned short int table[256];
    static unsigned short int marker = 255;
    const char* ch;

    /* Prepare the lookup table */
    if (marker == 255) {
        memset(table, 0, 256);
        marker = 1;
    } else {
        marker++;
    }
    for (ch = bad; *ch != 0; ch++)
        table[int(*ch)] = marker;

    /* Test the string */
    for (ch = str; *ch != 0; ch++)
        if (table[int(*ch)] == marker)
            return -1;

    return 1;
}

