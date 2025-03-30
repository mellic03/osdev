#pragma once
#include "vec.hpp"





template <typename T>
T& vec<2, T>::operator[]( size_t i )
{
    // return *(&x + i);
    // static_assert(i < 2);
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
    }
}

template <typename T>
T const& vec<2, T>::operator[]( size_t i ) const
{
    // return *(&x + i);
    // static_assert(i < 2);
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
    }
}












template <typename T>
T& vec<3, T>::operator[]( size_t i )
{
    // static_assert(i < 3);
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
                 case 2: return z;
    }
}

template <typename T>
T const& vec<3, T>::operator[]( size_t i ) const
{
    // static_assert(i < 3);
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
                 case 2: return z;
    }
}


template <typename T> vec<3, T>::vec( const vec<2,T> &xy, T z )
: vec(xy[0], xy[1], z) { }

template <typename T> vec<3, T>::vec( T x, const vec<2,T> &yz )
: vec(x, yz[0], yz[1]) { }











template <typename T> vec<4, T>::vec( const vec<2,T> &xy, T z, T w )
: vec(xy[0], xy[1], z, w) { }

template <typename T> vec<4, T>::vec( T x, T y, const vec<2,T> &zw )
: vec(x, y, zw[0], zw[1]) { }

template <typename T> vec<4, T>::vec( const vec<2,T> &xy, const vec<2,T> &zw )
: vec(xy[0], xy[1], zw[0], zw[1]) { }

template <typename T> vec<4, T>::vec( const vec<3,T> &xyz, T w )
: vec(xyz[0], xyz[1], xyz[2], w) { }

template <typename T> vec<4, T>::vec( T x, const vec<3,T> &yzw )
: vec(x, yzw[0], yzw[1], yzw[2]) { }


template <typename T>
T& vec<4, T>::operator[]( size_t i )
{
    // static_assert(i < 4);
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
                 case 2: return z;
                 case 3: return w;
    }
}

template <typename T>
T const& vec<4, T>::operator[]( size_t i ) const
{
    // static_assert(i < 4);
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
                 case 2: return z;
                 case 3: return w;
    }
}
