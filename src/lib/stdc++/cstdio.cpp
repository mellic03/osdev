#include <stdc++/cstdio.hpp>
#include <stdc++/cstdlib.hpp>
#include <stdc++/cstring.hpp>



// void std::putc( char c )
// {
// 	// ckTerminal_putc(c);
// }


// void std::puts( const char *s )
// {
//     // ckTerminal_puts(s);
// }


void puti( int n )
{
    char buf[16];
    std::itoa(n, buf, 10);
    std::puts(buf);
}


void putu( uint32_t n )
{
    char buf[16];
    std::itoa(n, buf, 10);
    std::puts(buf);
}


// void putf( double f )
// {
//     if (f < 0)
//     {
//         std::putc('-');
//         f = -f;
//     }

//     int whole = (int)f;
//     int frac = (int)(((f - (double)whole) * 100000.0) + 0.5);

//     if (frac >= 100000)
//     {
//         whole += 1;
//         frac = 0;
//     }

//     puti(whole);
//     std::putc('.');
//     puti(frac);
// }





size_t count_fmt_specifiers( const char *format )
{
    size_t num_spec = 0;

    while (*format)
    {
        if (*format == '%')
        {
            num_spec += 1;
            format += 1;
        }
        format += 1;
    }

    return num_spec;
}




static void long_printf( const char *&fmt, va_list &args )
{
    static char buf[32];
    int idx = 0;

    fmt++;

    switch (*fmt)
    {
        default: break;
        case 'd': idx = std::itoa(va_arg(args,      int), buf, 10); break;
        case 'u': idx = std::utoa(va_arg(args, uint32_t), buf, 10); break;
        case 'x': idx = std::itoa(va_arg(args, uint32_t), buf, 16); break;
    }

    if (idx)
    {
        buf[idx] = '\0';
    }
}


static void internal_printf( const char *&fmt, va_list &args )
{
    static char buf[32];
    int idx = 0;
    int lng = 0;

    fmt++;

    switch (*fmt)
    {
        default: break;

        // case 'l': lng = 1; break;

        case 'c': std::putc(va_arg(args, int));           break;
        case 's': std::puts(va_arg(args, const char *));  break;

        case 'd': idx = std::itoa(va_arg(args,      int), buf, 10); break;
        case 'u': idx = std::utoa(va_arg(args, uint32_t), buf, 10); break;
        case 'x': idx = std::itoa(va_arg(args, uint32_t), buf, 16); break;
    }

    if (idx)
    {
        buf[idx] = '\0';
        std::puts(buf);
    }

    else if (lng)
    {
        long_printf(fmt, args);
    }

    else if (idx && lng)
    {
        // error
    }

}


int std::printf( const char *fmt, ... )
{
    va_list args;
    size_t num_spec = count_fmt_specifiers(fmt);

    va_start ( args, fmt );

    while (*fmt)
    {
        if (*fmt == '%')
        {
            internal_printf(fmt, args);
        }

        else
        {
            std::putc(*fmt);
        }

        fmt += 1;
    }

    va_end( args );

    return num_spec;
}






