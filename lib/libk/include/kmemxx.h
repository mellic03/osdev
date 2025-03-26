#pragma once
#include <AAA.h>


#include <ZZZ.h>


#ifdef __cplusplus

template <typename T>
inline constexpr
void *kmemcpy( void *dst, const void *src, size_t count )
{
    auto *d = (T*)dst;
    auto *s = (T*)src;
    for (size_t i=0; i<count; i++)
        *(d++) = *(s++);
    return dst;
}

template <typename T>
inline constexpr
void *kmemset( void *dst, T value, size_t count )
{
    auto *d = (T*)dst;
    for (size_t i=0; i<count; i++)
        *(d++) = value;
    return dst;
}

// template <typename T>
// inline constexpr
// T *kmemcpy( T *d, const T *s, size_t n )
// {
//     for (size_t i=0; i<n; i++)
//         *(d++) = *(s++);
//     return d;
// }

// template <typename T>
// inline constexpr
// T *kmemset( T *d, T v, size_t n )
// {
//     for (size_t i=0; i<n; i++)
//         *(d++) = v;
//     return d;
// }

#endif

