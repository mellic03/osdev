#pragma once

#include <stddef.h>
#include <stdint.h>
#include <algorithm>


template <typename T, size_t N>
struct tvec
{
    T data[N];
    T &operator[]( int i ) { return data[i]; }
    const T &operator[]( int i ) const { return data[i]; }
};

