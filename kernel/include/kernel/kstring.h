#pragma once
#include "AAA.h"

// const char *skip_ch( const char *s, char ch );
// const char *seek_ch( const char *s, char ch );
// const char *skip_brk( const char *s, const char *brk );
// const char *seek_brk( const char *s, const char *brk );

// size_t count_ch( const char *s, char ch );



#include <string.h>

// skip past leading occurences of ch
inline const char *skip_ch( const char *str, char ch )
{
    while (*str && (*str == ch))
        str++;
    return str;
}

// Skip to next occurences of ch
inline const char *seek_ch( const char *str, char ch )
{
    while (*str && (*str != ch))
        str++;
    return str;
}

inline size_t count_ch( const char *s, char ch )
{
    size_t n = 0;

    while (*s)
    {
        if (*s == ch)
            n++;
        s++;
    }

    return n;
}



inline const char *skip_brk( const char *s, const char *brk )
{
    // size_t slen = strlen(s);
    size_t blen = strlen(brk);

    while (*s)
    {
        bool good = false;

        for (size_t i=0; i<blen; i++)
        {
            if (*s == brk[i])
            {
                good = true;
            }
        }

        if (!good)
        {
            return s;
        }

        s++;
    }

    return s;
}



inline const char *seek_brk( const char *s, const char *brk )
{
    size_t slen = strlen(s);
    size_t blen = strlen(brk);

    auto *nearest = s + slen;

    for (size_t i=0; i<blen; i++)
    {
        auto *ptr = seek_ch(s, brk[i]);
    
        if ((ptr - s) < (nearest - s))
        {
            nearest = ptr;
        }
    }
    
    return nearest;
}






#include "ZZZ.h"
