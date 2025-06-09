#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef __uint128_t uint128_t;
typedef __int128_t  int128_t;
typedef __float128  float128_t;


#ifdef __cplusplus
    #define ENUM_DTYPE(__dtype__) : __dtype__
#else
    #define nullptr NULL
    #define ENUM_DTYPE(__dtype__)
#endif

