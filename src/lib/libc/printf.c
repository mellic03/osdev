#include "stdio.h"

#include <stdint.h>
#include <stddef.h>

#include <kernel/terminal.h>


void putc( char c )
{
	terminal_putc(c);
}


void puts( const char *s )
{
    while (*s)
    {
        putc(*s);
        s++;
    }
}


void puti( int n )
{
    if (n == 0)
    {
		putc('0');
        return;
    }

    uint64_t i = n;
    uint64_t len = 0;
    while (i > 0)
    {
        i /= 10;
        len += 1;
    }

    char str[64];
    uint64_t idx = len-1;

    while (n > 0)
    {
        uint64_t c = n % 10;
        str[idx] = (char)(c + '0');
        idx -= 1;
        n /= 10;
    }

    str[len] = '\0';
    puts(str);
}







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


void internal_printf( char format, va_list args )
{
    switch (format)
    {
        case 'c': putc(va_arg(args, int));           break;
        case 's': puts(va_arg(args, const char *));  break;
     	case 'd': puti(va_arg(args, int));           break;
    }
}



int printf( const char *fmt, ... )
{
    va_list args;
    size_t num_spec = count_fmt_specifiers(fmt);

    va_start ( args, num_spec );

    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt += 1;
            internal_printf(*fmt, args);
        }

        else
        {
            putc(*fmt);
        }

        fmt += 1;
    }

    va_end( args );

    return num_spec;
}




