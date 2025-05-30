#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3
#define BUFSIZ (512*sizeof(char))
#define EOF -1

typedef uint64_t FILE;
extern FILE *stderr;    // Standard error output stream.
extern FILE *stdin;     // Standard input stream.
extern FILE *stdout;    // Standard output stream.


int     fflush( FILE* );
int     fgetc( FILE* );
char   *fgets( char *str, int n, FILE* );
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
int     snprintf( char *buf, size_t bufsz, const char *fmt, ... );

int     vprintf( const char *fmt, va_list args );
int     vfprintf( FILE*, const char *fmt, va_list args );
int     vsprintf( char *buf, const char *fmt, va_list args );
int     vsnprintf( char *buf, size_t bufsz, const char *fmt, va_list args );

// #define asprintf sprintf
int     asprintf( char **buf, const char *fmt, ... );


#ifdef __cplusplus
}
#endif

