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
size_t   strlen( const char* );
char    *strpbrk( const char *dst, const char *breakset );
size_t   strspn( const char *str, const char *ct );
size_t   strcspn( const char *str, const char *bad );
char    *strtok( char *str, const char *delim );

void    *memchr( const void *ptr, int ch, size_t count );
int      memcmp( const void*, const void*, size_t n );
void    *memcpy( void *dst, const void *src, size_t n );
void    *memmove( void *dst, const void *src, size_t n );
void    *memset( void *s, uint8_t c, size_t n );
void    *memcpy32( uint32_t *dst, const uint32_t *src, size_t count );
void    *memset32( uint32_t *src, uint32_t value, size_t count );


#ifdef __cplusplus
}
#endif
