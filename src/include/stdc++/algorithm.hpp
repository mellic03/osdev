#pragma once

// #include <cstddef>
// #include <cstdint>
#include <stddef.h>
#include <stdint.h>




namespace std
{
    template <typename T>
    constexpr const T& min( const T&, const T& );

    template <typename T>
    constexpr const T& max( const T&, const T& );

    template <typename T>
    constexpr const T& clamp( const T &X, const T &lo, const T &hi );

}



#include "algorithm.inl"

