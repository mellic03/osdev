#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <idk/idk_file.h>
typedef idk_file_t FILE;


#ifdef __cplusplus
extern "C" {
#endif


#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3
#define BUFSIZ (512*sizeof(char))
#define EOF -1


extern FILE *stderr;    // Standard error output stream.
extern FILE *stdin;     // Standard input stream.
extern FILE *stdout;    // Standard output stream.


int __libc_stdio_init( FILE*, FILE*, FILE* );
// int __libc_stdio_init();


int     fflush( FILE* );
int     fputc( char, FILE* );
int     fputs( const char*, FILE* );
int     fprintf( FILE*, const char *fmt, ... );
int     fseek( FILE*, long offset, int origin );
size_t  fwrite( const void *buf, size_t size, size_t count, FILE* );

int     getc ( FILE* );

int     putc( char );
int     puts( const char* );
int     printf( const char *fmt, ... );

int     sprintf( char *buf, const char *fmt, ... );

int     vprintf( const char *fmt, va_list args );
int     vfprintf( FILE*, const char *fmt, va_list args );
int     vsprintf( char *buf, const char *fmt, va_list args );

#ifdef __cplusplus
}
#endif

