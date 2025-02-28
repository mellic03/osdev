#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>


namespace std
{
    void putc( char );
    void puts( const char* );
    int  printf( const char *fmt, ... );


    struct FILE;
    FILE *fopen( const char *filename, const char *mode );

}
