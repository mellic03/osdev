#include <stdc/string.h>
#include <stdc++/cstring.hpp>


size_t strlen( const char *str ) 
{
    return std::strlen( str ) ;
}


void *memcpy( void *dst, const void *src, size_t n )
{
    return std::memcpy(dst, src, n);
}


void *memset( void *s, uint8_t c, size_t n )
{
    return std:: memset(s, c, n);
}


void *memmove( void *dst, const void *src, size_t n )
{
    return std:: memmove( dst, src, n );
}


int memcmp( const void *s1, const void *s2, size_t n )
{
    return std::memcmp(s1, s2, n );
}



