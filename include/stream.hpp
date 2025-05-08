#pragma once
#include <stddef.h>
#include <string.h>


inline void *streamwrite( void *dst, void *src, size_t size )
{
    memcpy(dst, src, size);
    return (void*)((uintptr_t)dst + size);
}

inline const void *streamread( const void *src, void *dst, size_t size )
{
    memcpy(dst, src, size);
    return (void*)((uintptr_t)src + size);
}


template <typename T>
inline void *stream_write( void *dst, const T &src )
{
    memcpy(dst, &src, sizeof(T));
    return (void*)((uintptr_t)dst + sizeof(T));
}

template <typename T>
inline const void *stream_read( const void *src, const T &dst )
{
    memcpy(&dst, src, sizeof(T));
    return (void*)((uintptr_t)src + sizeof(T));
}

