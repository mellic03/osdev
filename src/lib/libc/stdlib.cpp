#include <stdlib.h>


int ltoa( int64_t value, char *sp, int radix )
{
    char tmp[32];
    char *tp = tmp;
    int64_t i;
    unsigned v;

    int64_t sign = (radix == 10 && value < 0);    
    v = (sign) ? -value : (uint32_t)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int64_t len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}



int ultoa( uint64_t value, char *sp, int radix )
{
    char tmp[32];
    char *tp = tmp;
    uint64_t i;
    uint64_t v = value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}



int itoa( int value, char *sp, int radix )
{
    char tmp[32];
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);    
    v = (sign) ? -value : (unsigned )value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}


int utoa( uint32_t value, char *sp, int radix )
{
    return ultoa((uint64_t)value, sp, radix);
}


