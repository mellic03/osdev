#ifndef CSTDIO_H
#define CSTDIO_H

#include <stdarg.h>
// #ifndef va_list
//     typedef __builtin_va_list va_list;
// #endif


void putc( char );
void puts( const char* );
void puti( int );
int printf( const char *fmt, ... );

const char *itoa( int );


#endif