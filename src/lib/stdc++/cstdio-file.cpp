#include <stdc++/cstdio.hpp>
#include <stdc++/cstdlib.hpp>


struct std::FILE
{
    uint8_t  *base;
    uint8_t  *end;
    uint32_t  cursor;
    uint32_t  closed;
};


// static std::FILE __ck_stdin;
// static std::FILE __ck_stdout;
// static std::FILE __ck_stderr;
// static std::FILE __ck_dummy;

// std::FILE *std::stdin  = &__ck_stdin;
// std::FILE *std::stdout = &__ck_stdout;
// std::FILE *std::stderr = &__ck_stderr;



// std::FILE*
// std::fopen( const char *filename, const char *mode )
// {
//     std::FILE *fh = &__ck_dummy;
//     fh->closed = false;

//     if (filename == 0 || mode[0] == 'a')
//     {
//         return fh;
//     }

//     return fh;
// }



// int
// std::fclose( FILE *fh )
// {
//     fh->closed = true;

//     return 0;
// }



// int std::__ck_vputc( char *&dst, char c )
// {
//     *dst = c;
//     dst++;

//     return c;
// }


// int std::__ck_vputs( char *&dst, const char *str )
// {
//     while (*str)
//     {
//         std::__ck_vputc(dst, *str);
//         str++;
//     }

//     return 1;  // success
// }


// int
// std::fputc( char c, FILE *fh )
// {
//     return std::__ck_vputc((void*)(fh->base+fh->cursor), c);
// }


// int
// std::fputs( const char *str, FILE *fh )
// {
//     return std::__ck_vputs((void*)(fh->base+fh->cursor), str);
// }


// static void __ck_vprintf_internal( void *addr, const char *&fmt, va_list &args );





// int
// std::__ck_vprintf( char *addr, const char *fmt, ... )
// {
//     va_list args;
//     va_start(args, fmt);

//     char *dst = static_cast<char*>(addr);

//     while (*fmt)
//     {
//         if (*fmt == '%')
//         {
//             __ck_vprintf_internal(dst, fmt, args);
//         }

//         else
//         {
//             std::__ck_vputc(dst, *fmt);
//         }

//         fmt += 1;
//     }

//     va_end( args );

//     return 0;
// }




// int 
// std::fprintf( FILE *fh, const char *fmt, ... )
// {
//     va_list args;
//     va_start(args, fmt);

//     int n = std::__ck_vprintf(
//         (void *)(fh->base + fh->cursor), fmt, args
//     );

//     va_end(args);

//     return 0;
// }




// static void __ck_vprintf_internal( char *&dst, const char *&fmt, va_list &args )
// {
//     static char buf[32];
//     int idx = 0;
//     // int lng = 0;

//     fmt++;

//     switch (*fmt)
//     {
//         default: break;

//         // case 'l': lng = 1; break;

//         case 'c': std::__ck_vputc(dst, va_arg(args, int));           break;
//         case 's': std::__ck_vputs(dst, va_arg(args, const char *));  break;

//         case 'd': idx = std::itoa(va_arg(args,      int), buf, 10); break;
//         case 'u': idx = std::utoa(va_arg(args, uint32_t), buf, 10); break;
//         case 'x': idx = std::utoa(va_arg(args, uint32_t), buf, 16); break;
//     }

//     if (idx)
//     {
//         buf[idx] = '\0';
//         std::__ck_vputs(dst, buf);
//     }

//     // else if (lng)
//     // {
//     //     long_printf(fmt, args);
//     // }

//     // else if (idx && lng)
//     // {
//     //     // error
//     // }

// }