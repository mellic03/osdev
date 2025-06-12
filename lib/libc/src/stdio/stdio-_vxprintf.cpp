#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <kernel/kstring.h>




#ifdef __SSE__
#include <math.h>
#include "ftoa.hpp"
// static double shitpow( double base, double exp )
// {
//     double result = 1.0;
//     for (double i=0.0; i<exp; i++)
//         result *= base;
//     return result;
// }


// static char *__dtoa( double f, int, char *buf )
// {
//     uint64_t whl = uint64_t(trunc(f));
//     uint64_t fct = uint64_t(1'000'000 * fabs(fract(f)));
//     // double  dfct = shitpow(10, precision) * f;
//     // int64_t sign = (f < 0.0) ? -1 : +1;
//     // int64_t whl  = sign*int64_t(trunc(f));
//     // double  dfct = float(1<<precision) * f;
//     // int64_t fct  = int64_t(1'000'000.0 * fabs(fract(f)));

//     // *(buf++) = (sign == -1) ? '-' : '+';
//     buf = ultoa(whl, buf, 10);
//     *(buf++) = '.';
//     buf = ultoa(fct, buf, 10);

//     return buf;
// }


static char *__vsprintf_sse( char *buf, const char *&fmt, va_list args )
{
    if (*fmt == 'f')
    {
        float value = (float)va_arg(args, double);
        return buf + ftoa(buf, 64, value, 16);
    }

    return buf;

}
#endif





static char *__vsprintf( char *buf, char *end, const char *&fmt, va_list args )
{
    // char *prevbuf = buf;
    // int dgt = 0;
    int lng = 0;
    fmt++;

    // if (isdigit(*fmt))
    // {
    //     dgt = (tolower(*fmt) - 'a');
    //     fmt++;
    // }

    switch (*fmt)
    {
        default: break;

        case 'l': lng = 1; break;
        case 'c': *(buf++) = va_arg(args, int);                   break;
        case 'd': buf  = itoa(va_arg(args,      int), buf, 10);   break;
        case 'u': buf  = utoa(va_arg(args, uint32_t), buf, 10);   break;
        case 'x': buf  = ultoa(va_arg(args, uint32_t), buf, 16);  break;
        case 's':
            if (end) buf += strlen(strncpy(buf, va_arg(args, const char*), end-buf-1));
            else     buf += strlen(strcpy(buf, va_arg(args, const char*)));
            break;
        #ifdef __SSE__
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
            case 'd': buf = ltoa(va_arg(args,  int64_t),  buf, 10); break;
            case 'u': buf = ultoa(va_arg(args, uint64_t), buf, 10); break;
            case 'x': buf = ultoa(va_arg(args, uint64_t), buf, 16); break;

            #ifdef __SSE__
                case '0': buf = __vsprintf_sse(buf, fmt, args); break;
                case '.': buf = __vsprintf_sse(buf, fmt, args); break;
                case 'f': buf = __vsprintf_sse(buf, fmt, args); break;
            #endif
        }
    }


    // if (dgt && (buf-prevbuf < dgt))
    // {
    //     int n = dgt - (buf-prevbuf);

    //     for (int i=0; i<n; i++)
    //     {
    //         *(buf++) = '0';
    //     }
    // }

    return buf;
}



int vprintf( const char *fmt, va_list args )
{
    return vfprintf(stdout, fmt, args);
}


int vfprintf( FILE*, const char*, va_list )
{
    // auto *stream = &((vfsFileEntry*)addr)->stream;
    // int n = vsprintf((char*)(&stream->m_base[stream->m_write]), fmt, args);
    // stream->m_write = (stream->m_write + n) % stream->m_size;
    // stream->flush();

    // #ifndef __is_kernel
    //     kthread::yield();
    // #endif
    // return n;

    return 0;
}


int vsprintf( char *buf, const char *fmt, va_list args )
{
    char *start = buf;

    while (*fmt)
    {
        if (*fmt == '%' && *(fmt+1) == '%')
            *(buf++) = *(fmt++);
        else if (*fmt == '%')
            buf = __vsprintf(buf, nullptr, fmt, args);
        else
            *(buf++) = *fmt;

        fmt += 1;
    }
    *(buf++) = '\0';

    return buf - start - 1;
}


int vsnprintf( char *buf, size_t bufsz, const char *fmt, va_list args )
{
    char *start = buf;
    char *end   = buf + bufsz;

    while (*fmt && (buf < end))
    {
        if (*fmt == '%')
            buf = __vsprintf(buf, end, fmt, args);
        else
            *(buf++) = *fmt;

        fmt += 1;
    }

    if (buf >= end)
    {
        *(end-1) = '\0';
        return bufsz;
    }

    *(buf++) = '\0';

    return buf - start - 1;
}

