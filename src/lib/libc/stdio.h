#ifndef CSTDIO_H
#define CSTDIO_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
// #ifndef va_list
//     typedef __builtin_va_list va_list;
// #endif


void putc( char );
void puts( const char* );
void puti( int );
int printf( const char *fmt, ... );

int itoa( int value, char *sp, int radix );
int utoa( uint32_t value, char *sp, int radix );




#ifdef __cplusplus
}
#endif


#endif // CSTDIO_H
