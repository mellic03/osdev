#include <kstring.h>
#include <kstring.hpp>
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

size_t count_ch( const char *s, char ch )
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



const char *skip_brk( const char *s, const char *brk )
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



const char *seek_brk( const char *s, const char *brk )
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


#include <kernel/log.hpp>

std::vector<std::string>&
kstring::tokenize( const char *str, char delim )
{
    static std::vector<std::string> out;
    // for (char *sep: out)
        // delete[] sep;
    out.clear();

    int len = strlen(str);
    auto *A = str;
    auto *B = A;

    syslog log("kstring::tokenize");
    while (B < str+len)
    {
        B = seek_ch(A+1, delim);
        out.push_back(std::string(A, B-A));
        log("A, B, len: \'%c\', \'%c\', %d", *A, *B, B-A);
        A = B+1;
    }

    return out;
}