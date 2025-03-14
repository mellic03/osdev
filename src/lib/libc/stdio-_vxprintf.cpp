#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *__dtoa( double f, char *buf )
{
    int whole = int(f);
    int fract = int(1000.0 * f);

    if (fract < 0)
    {
        fract = -fract;
    }

    buf = itoa(whole, buf, 10);
    *(buf++) = '.';
    buf = itoa(fract, buf, 10);

    return buf;
}


static char *__internal_vsprintf( char *buf, const char *&fmt, va_list args )
{
    // char tmp[32]; // = __ck_libc_fprintf_buf;
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
        case 'f': buf = __dtoa(va_arg(args, double), buf);       break;
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
        }
    }

    return buf;
}



// int vfprintf( FILE *fh, const char *fmt, va_list args )
// {
//     while (*fmt)
//     {
//         if (*fmt == '%')
//         {
//             __internal_fprintf(fh, fmt, args);
//         }

//         else
//         {
//             fputc(*fmt, fh);
//         }

//         fmt += 1;
//     }

//     va_end( args );

//     flush_stdout();
//     memset(fh->data, '\0', (fh->tail - fh->data));
//     fh->tail = fh->data;

//     return 1;
// }



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
            buf = __internal_vsprintf(buf, fmt, args);
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

