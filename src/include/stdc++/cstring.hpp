#pragma once

// #include <cstddef>
// #include <cstdint>
#include <stddef.h>
#include <stdint.h>


namespace std
{
    size_t strlen( const char *str );
    int    strcmp( const char*, const char* );

    void *memcpy( void *dst, const void *src, size_t N );

    /**
     * Set N bytes of s to C
     */
    void *memset( void *s, uint8_t c, size_t N );

    void *memmove( void *dst, const void *src, size_t N );

    int   memcmp( const void *s1, const void *s2, size_t N );

}

// void std::putc( char c )
// {
// 	ckTerminal_putc(c);
// }


// void std::puts( const char *s )
// {
//     ckTerminal_puts(s);
// }