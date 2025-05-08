#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
    #define retqual inline constexpr
#else
    #define retqual inline
#endif


retqual int isdigit( int c ) { return (('0' <= c) && (c <= '9')); }
retqual int isalpha( int c ) { return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')); }
retqual int isalnum( int c ) { return (isalpha(c) || isdigit(c)); }
retqual int islower( int c ) { return (('a' <= c) && (c <= 'z')); }
retqual int isupper( int c ) { return (('A' <= c) && (c <= 'Z')); }

retqual int tolower( int c ) { return (islower(c)) ? c : c+32; }
retqual int toupper( int c ) { return (isupper(c)) ? c : c-32; }


