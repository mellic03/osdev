#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <ck/ck_file.h>
typedef ckFILE FILE;


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


int __libc_stdio_init( void* );


int fseek( FILE*, long offset, int origin );

int fputc( char, FILE* );
int fputs( const char*, FILE* );
int fprintf( FILE*, const char *fmt, ... );
int sprintf( char *buf, const char *fmt, ... );


size_t fwrite( const void *buf, size_t size, size_t count, FILE* );


int putc( char );
int puts( const char* );
int printf( const char *fmt, ... );

int getc ( FILE* );



#ifdef __cplusplus
}
#endif

