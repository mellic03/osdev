#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int atoi_r( const char *str, int radix )
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


int atoi( const char *src )
{
    size_t len = strlen(src);
    char *str = (char*)malloc(len);
    strcpy(str, src);

    int radix = 10;

    if (isdigit(str[0]) && isdigit(str[1]))
    {
        radix = 10;
    }

    else if (str[0]=='0' && str[1]=='o')
    {
        radix = 8;
        str += 2;
    }

    else if (str[0]=='0' && str[1]=='x')
    {
        radix = 16;
        str += 2;

        for (size_t i=0; i<len; i++)
        {
            if (isalpha(str[i]))
            {
                str[i] = tolower(str[i]);
            }
        }
    }

    return atoi_r(str, radix);
}

