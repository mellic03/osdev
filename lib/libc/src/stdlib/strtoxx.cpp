#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static int atoi_r( const char *str, int radix )
{
    int len = int(strlen(str));

    int result = 0;
    int power  = 1;

    for (int i=len-1; i>=0; i--)
    {
        int c = int(str[i]) - int('0');

        if (isalpha(str[i]))
        {
            c -= 'a';
        }

        if (c == ' ')
        {
            continue;
        }

        if (0 <= c && c < radix)
        {
            result += power * c;
        }
        
        power *= radix;
    }

    return result;
}



uint32_t strtol( const char *str, char **, int )
{
    static char buf[128];

    size_t len = strlen(str);

    if (len > 127)
        return ~(uint32_t(0));
    strcpy(buf, str);

    if (isdigit(buf[0]) && isdigit(buf[1]))
    {
        return atoi_r(buf, 10);
    }

    int radix = 10;

    if (strcmp(buf, "0b"))
    {
        radix = 2;
    }

    else if (strcmp(buf, "0o"))
    {
        radix = 8;
    }

    else if (strcmp(buf, "0x"))
    {
        radix = 16;

        for (size_t i=0; i<len; i++)
        {
            if (isalpha(buf[i]))
            {
                buf[i] = tolower(buf[i]);
            }
        }
    }

    return atoi_r(buf+2, radix);
}


