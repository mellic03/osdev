#pragma once

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

inline int isdigit( int c ) { return ('0' <= c && c <= '9'); }
inline int isalpha( int c ) { return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')); }
inline int isalnum( int c ) { return (isalpha(c) || isdigit(c)); }
inline int islower( int c ) { return ('a' <= c && c <= 'z'); }
inline int isupper( int c ) { return ('A' <= c && c <= 'Z'); }

#ifdef __cplusplus
}
#endif

