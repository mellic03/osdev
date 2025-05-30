#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char    *strchr( const char *str, int ch );
char    *antistrchr( const char *str, int ch );

int      strcmp( const char*, const char* );
int      strncmp( const char*, const char*, size_t );
char    *strcpy( char*, const char* );
char    *stpcpy( char*, const char* );
char    *strdup( const char* );
char    *strncpy( char*, const char*, size_t );
size_t   strlen( const char* );
char    *strpbrk( const char *dst, const char *breakset );
size_t   strspn( const char *str, const char *ct );
char    *strstr( char const *str, char const *substr );
size_t   strcspn( const char *str, const char *bad );
char    *strtok( char *str, const char *delim );

void    *memchr( const void *ptr, int ch, size_t count );
int      memcmp( const void*, const void*, size_t n );
void    *memcpy( void *dst, const void *src, size_t n );
void    *memmove( void *dst, const void *src, size_t n );
void    *memset( void *s, uint8_t c, size_t n );


#ifdef __cplusplus
}
#endif
