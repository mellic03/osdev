#include <stdc++/cstdio.hpp>
#include <stdc++/cstdlib.hpp>
#include <stdc++/cstring.hpp>

#include "./cstdio-file.cpp"



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




// static void long_printf( const char *&fmt, va_list &args )
// {
//     static char buf[32];
//     int idx = 0;

//     fmt++;

//     switch (*fmt)
//     {
//         default: break;
//         case 'd': idx = std::itoa(va_arg(args,      int), buf, 10); break;
//         case 'u': idx = std::utoa(va_arg(args, uint32_t), buf, 10); break;
//         case 'x': idx = std::itoa(va_arg(args, uint32_t), buf, 16); break;
//     }

//     if (idx)
//     {
//         buf[idx] = '\0';
//     }
// }



// int std::printf( const char *fmt, ... )
// {
//     va_list args;
//     va_start(args, fmt);
//     int res = std::fprintf(std::stdout, fmt, args);
//     va_end(args);
//     return res;
// }

typedef int (*__ck_putc_method_t)( char  );
typedef int (*__ck_puts_method_t)( const char* );


static struct {
    __ck_putc_method_t __putc = nullptr;
    __ck_puts_method_t __puts = nullptr;
} __ck_syscall;



void std::cstdio_init( void *__putc_ptr, void *__puts_ptr )
{
    __ck_syscall.__putc = (__ck_putc_method_t)(__putc_ptr);
    __ck_syscall.__puts = (__ck_puts_method_t)(__puts_ptr);
}



int std::putc( char c )
{
    return __ck_syscall.__putc(c);
}


int std::puts( const char *s )
{
    return __ck_syscall.__puts(s);
}





static void internal_printf( const char *&fmt, va_list &args )
{
    static char buf[32];
    int idx = 0;
    // int lng = 0;

    fmt++;

    switch (*fmt)
    {
        default: break;

        // case 'l': lng = 1; break;

        case 'c': std::putc(va_arg(args, int));           break;
        case 's': std::puts(va_arg(args, const char *));  break;

        case 'd': idx = std::itoa(va_arg(args,  int32_t), buf, 10); break;
        case 'u': idx = std::utoa(va_arg(args, uint32_t), buf, 10); break;
        case 'x': idx = std::utoa(va_arg(args, uint32_t), buf, 16); break;
    }

    if (idx)
    {
        buf[idx] = '\0';
        std::puts(buf);
    }

    // else if (lng)
    // {
    //     long_printf(fmt, args);
    // }

    // else if (idx && lng)
    // {
    //     // error
    // }

}


int std::printf( const char *fmt, ... )
{
    va_list args;
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

    return 1;
}