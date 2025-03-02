#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


size_t   strlen( const char* );
int      strcmp( const char*, const char* );
void    *memcpy( void *dst, const void *src, size_t n );
void    *memset( void *s, uint8_t c, size_t n );
void    *memmove( void *dst, const void *src, size_t n );
int      memcmp( const void*, const void*, size_t n );




#ifdef __cplusplus
}
#endif

