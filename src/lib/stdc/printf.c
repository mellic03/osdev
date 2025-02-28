// #include "stdio.h"
// #include "string.h"

// #include <stdint.h>
// #include <stddef.h>


// void putc( char c )
// {
// 	// ckTerminal_putc(c);
// }


// void puts( const char *s )
// {
//     // ckTerminal_puts(s);
// }


// void puti( int n )
// {
//     char buf[16];
//     itoa(n, buf, 10);
//     puts(buf);
// }


// void putu( uint32_t n )
// {
//     char buf[16];
//     itoa(n, buf, 10);
//     puts(buf);
// }


// void putf( double f )
// {
//     if (f < 0)
//     {
//         putc('-');
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
//     putc('.');
//     puti(frac);
// }





// size_t count_fmt_specifiers( const char *format )
// {
//     size_t num_spec = 0;

//     while (*format)
//     {
//         if (*format == '%')
//         {
//             num_spec += 1;
//             format += 1;
//         }
//         format += 1;
//     }

//     return num_spec;
// }


// // void internal_printf( char format, va_list *args )
// // {
// //     switch (format)
// //     {
// //         case 'c': putc(va_arg(*args, int));           break;
// //         case 's': puts(va_arg(*args, const char *));  break;
// //      	case 'd': puti(va_arg(*args, int));           break;
// //      	case 'u': putu(va_arg(*args, uint32_t));       break;
// //      	case 'f': putf(va_arg(*args, double));         break;
// //     }
// // }



// int printf( const char *fmt, ... )
// {
//     va_list args;
//     size_t num_spec = count_fmt_specifiers(fmt);

//     va_start ( args, fmt );

//     char itoabuf[32];
//     memset(itoabuf, '\0', 32);

//     while (*fmt)
//     {
//         if (*fmt == '%')
//         {
//             fmt += 1;

//             switch (*fmt)
//             {
//                 default: break;
//                 case 'c': putc(va_arg(args, int));           break;
//                 case 's': puts(va_arg(args, const char *));  break;
//                 case 'f': putf(va_arg(args, double));        break;

//                 case 'x':
//                     itoa(va_arg(args, int), itoabuf, 16);
//                     puts(itoabuf);
//                     break;

//                 case 'd':
//                     itoa(va_arg(args, int), itoabuf, 10);
//                     puts(itoabuf);
//                     break;

//                 case 'u':
//                     utoa(va_arg(args, uint32_t), itoabuf, 10);
//                     puts(itoabuf);
//                     break;
//             }
//         }

//         else
//         {
//             putc(*fmt);
//         }

//         fmt += 1;
//     }

//     va_end( args );

//     return num_spec;
// }




