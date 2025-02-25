#include "stdio.h"

#include <stdint.h>
#include <stddef.h>


void putc( char c )
{
    const uint16_t *vga = (uint16_t*)(0xB8000);
	// terminal_color = vga_entry_color(global_fg, global_bg); // First byte is foreground color, second byte is background color
}


void puts( const char *s )
{
    while (*s)
    {
        putc(*s);
        s++;
    }
}


int printf( const char *fmt, ... )
{
    puts(fmt);
    return 0;
    // va_list args;
    // size_t num_spec = num_format_specs(format);

    // va_start ( args, num_spec );

    // while (*format)
    // {
    //     if (*format == '%')
    //     {
    //         format += 1;
    //         internal_printf(*format, args);
    //     }

    //     else
    //     {
    //         system::terminal::putchar(*format);
    //     }

    //     format += 1;
    // }

    // va_end( args );

    // return num_spec;
}
