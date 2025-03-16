#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


size_t   strlen( const char* );
int      strcmp( const char*, const char* );
int      strncmp( const char*, const char*, size_t );
char    *strcpy( char*, const char* );
void    *memcpy( void *dst, const void *src, size_t n );
void    *memset( void *s, uint8_t c, size_t n );
void    *memmove( void *dst, const void *src, size_t n );
int      memcmp( const void*, const void*, size_t n );

void    *memcpy32( uint32_t *dst, const uint32_t *src, size_t count );
void    *memset32( uint32_t *src, uint32_t value, size_t count );


#ifdef __cplusplus
}
#endif

