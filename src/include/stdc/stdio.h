#pragma once
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif




void putc( char );
void puts( const char* );
void puti( int );
int  printf( const char *fmt, ... );




#ifdef __cplusplus
}
#endif

