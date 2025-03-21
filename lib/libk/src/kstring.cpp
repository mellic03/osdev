#include <kstring.h>
#include <string.h>


// skip past leading occurences of ch
const char *skip_ch( const char *s, char ch )
{
    while (*s == ch)
    {
        s++;
    }

    return s;
}

// Skip to next occurences of ch
const char *seek_ch( const char *s, char ch )
{
    while (*s && *s != ch)
    {
        s++;
    }

    return s;
}



const char *skip_brk( const char *s, const char *brk )
{
    size_t len = strlen(brk);
    bool good = true;

    while (good)
    {
        for (size_t i=0; i<len; i++)
        {
            good = good || (*s == brk[i]);
        }
    
        s++;
    }

    return s;
}



const char *seek_brk( const char *s, const char *brk )
{
    size_t slen = strlen(brk);
    size_t blen = strlen(brk);

    const char *nearest = s + slen;

    for (size_t i=0; i<blen; i++)
    {
        const char *ptr = seek_ch(s, brk[i]);
        if (ptr < nearest)
        {
            nearest = ptr;
        }
    }

    return nearest;
}




#include <ZZZ.h>
