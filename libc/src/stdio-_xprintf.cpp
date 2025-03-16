// #include "../include/stdio.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>


int printf( const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    int n = vprintf(fmt, args);
    va_end(args);
    return n;
}


int fprintf( FILE *fh, const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    int n = vfprintf(fh, fmt, args);
    va_end(args);
    return n;
}


int sprintf( char *buf, const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    int n = vsprintf(buf, fmt, args);
    va_end(args);
    return n;
}

