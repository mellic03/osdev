#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>


namespace std
{
    struct FILE;

    extern FILE *stdin;
    extern FILE *stdout;
    extern FILE *stderr;

    void  cstdio_init( void *__putc_impl, void *__puts_impl );

    FILE *fopen( const char *filename, const char *mode );
    int   fclose( FILE* );
    // size_t fwrite( const void *buf, size_t size, size_t count, FILE* );
    // size_t fread( void *buf, size_t size, size_t nmemb, FILE* );

    // int __ck_vputc( char *&dst, char c );
    // int __ck_vputs( char *&dst, const char* );
    // int __ck_vprintf( void *addr, const char *fmt, ... );

    // int fputc( char, FILE* );
    // int fputs( const char*, FILE* );
    // int fprintf( FILE*, const char *fmt, ... );

    int putc( char );
    int puts( const char* );
    int printf( const char *fmt, ... );
  
}
