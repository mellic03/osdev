#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <idk_file.h>


static double shitpow( double base, double exp )
{
    double result = 1.0;

    for (double i=0.0; i<exp; i++)
    {
        result *= base;
    }

    return result;
}

static char *__dtoa( double f, int precision, char *buf )
{
    uint64_t whole  = uint64_t(f);
    double   dfract = shitpow(10, precision) * f;
    uint64_t fract  = uint64_t((dfract < 0.0) ? -dfract : dfract);

    buf = ultoa(whole, buf, 10);
    *(buf++) = '.';
    buf = ultoa(fract, buf, 10);

    return buf;
}

static const char *strseek( const char *str, char c )
{
    while (*str && (*str != c))
    {
        str++;
    }

    if (!(*str))
    {
        return nullptr;
    }

    return str;
}

static char *__vsprintf_sse( char *buf, const char *&fmt, va_list args )
{
    // printf("%0.5f")

    if (*fmt == 'f')
    {
        return __dtoa(va_arg(args, double), 8, buf);
    }


    auto *start = fmt;
    auto *end   = strseek(fmt, 'f');

    if (!end)
    {
        return buf;
    }


    // have isolated from '0' to 'f'

    // Implement %00f later
    if (*(start+1) != '.')
    {
        return buf;
    }


    start++; // *start == '.'
    start++; // *start == digit

    int precision = int(*start);
    return __dtoa(va_arg(args, double), precision, buf);
}



static char *__vsprintf( char *buf, const char *&fmt, va_list args )
{
    int lng = 0;
    fmt++;

    switch (*fmt)
    {
        default: break;

        case 'l': lng = 1; break;

        case 'c': *(buf++) = va_arg(args, int);                  break;
        case 's': buf = strcpy(buf, va_arg(args, const char *)); break;
        case 'd': buf = itoa(va_arg(args,      int), buf, 10);   break;
        case 'u': buf = utoa(va_arg(args, uint32_t), buf, 10);   break;
        case 'x': buf = ultoa(va_arg(args, uint32_t), buf, 16);  break;

        #ifdef IDK_SSE
            case '0': buf = __vsprintf_sse(buf, fmt, args); break;
            case '.': buf = __vsprintf_sse(buf, fmt, args); break;
            case 'f': buf = __vsprintf_sse(buf, fmt, args); break;
        #endif
    }

    if (lng)
    {
        fmt++;

        switch (*fmt)
        {
            default: break;
            case 'd': buf = ltoa(va_arg(args,  int64_t), buf, 10);  break;
            case 'u': buf = ultoa(va_arg(args, uint64_t), buf, 10); break;
            case 'x': buf = ultoa(va_arg(args, uint64_t), buf, 16); break;

            #ifdef IDK_SSE
                case '0': buf = __vsprintf_sse(buf, fmt, args); break;
                case '.': buf = __vsprintf_sse(buf, fmt, args); break;
                case 'f': buf = __vsprintf_sse(buf, fmt, args); break;
            #endif
        }
    }

    return buf;
}



int
vprintf( const char *fmt, va_list args )
{
    return vfprintf(stdout, fmt, args);
}


int
vfprintf( FILE *fh, const char *fmt, va_list args )
{
    char *buf = idk_file_data(fh);
    return vsprintf(buf, fmt, args);
}


int
vsprintf( char *buf, const char *fmt, va_list args )
{
    char *start = buf;

    while (*fmt)
    {
        if (*fmt == '%')
        {
            buf = __vsprintf(buf, fmt, args);
        }

        else
        {
            *(buf++) = *fmt;
        }

        fmt += 1;
    }

    *(buf++) = '\0';

    return buf - start;
}

