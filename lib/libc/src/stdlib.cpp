#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include <kmalloc.h>


void *malloc( size_t nbytes )
{
    return kmalloc(nbytes);
}

void free( void *ptr )
{
    kfree(ptr);
}


char *ltoa( int64_t value, char *buf, int radix )
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
        *buf++ = '-';
        len++;
    }

    while (tp > tmp)
        *buf++ = *--tp;

    return buf;
}



char *ultoa( uint64_t value, char *buf, int radix )
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
          *tp++ = i + 'A' - 10;
    }

    while (tp > tmp)
        *buf++ = *--tp;

    return buf;
}



char *itoa( int value, char *buf, int radix )
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
        *buf++ = '-';
        len++;
    }

    while (tp > tmp)
        *buf++ = *--tp;

    return buf;
}


char *utoa( uint32_t value, char *buf, int radix )
{
    return ultoa((uint64_t)value, buf, radix);
}


