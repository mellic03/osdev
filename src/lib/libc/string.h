#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>


size_t strlen( const char *str );


void *memcpy( void *dst, const void *src, size_t N );

/**
 * Set N bytes of s to C
 */
void *memset( void *s, uint8_t c, size_t N );
void *memmove( void *dst, const void *src, size_t N );
int   memcmp( const void *s1, const void *s2, size_t N );

#ifdef __cplusplus
}
#endif

