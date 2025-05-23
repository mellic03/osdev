#pragma once
#include "vec.hpp"





template <typename T>
T& vec<1, T>::operator[]( size_t )
{
    return x;
}

template <typename T>
T const& vec<1, T>::operator[]( size_t ) const
{
    return x;
}





template <typename T>
T& vec<2, T>::operator[]( size_t i )
{
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
    }
}

template <typename T>
T const& vec<2, T>::operator[]( size_t i ) const
{
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
    }
}

template <typename T> vec<2, T>::vec( const vec<3,T> &xy )
: vec(xy[0], xy[1]) { }

template <typename T> vec<2, T>::vec( const vec<4,T> &xy )
: vec(xy[0], xy[1]) { }









template <typename T>
T& vec<3, T>::operator[]( size_t i )
{
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
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
                 case 2: return z;
    }
}


template <typename T> vec<3, T>::vec( const vec<2,T> &xy, T _z )
: vec(xy[0], xy[1], _z) { }

template <typename T> vec<3, T>::vec( T _x, const vec<2,T> &yz )
: vec(_x, yz[0], yz[1]) { }

template <typename T> vec<3, T>::vec( const vec<4,T> &xyz )
: vec(xyz[0], xyz[1], xyz[2]) { }











template <typename T> vec<4, T>::vec( const vec<2,T> &xy, T _z, T _w )
: vec(xy[0], xy[1], _z, _w) { }

template <typename T> vec<4, T>::vec( T _x, T _y, const vec<2,T> &zw )
: vec(_x, _y, zw[0], zw[1]) { }

template <typename T> vec<4, T>::vec( const vec<2,T> &xy, const vec<2,T> &zw )
: vec(xy[0], xy[1], zw[0], zw[1]) { }

template <typename T> vec<4, T>::vec( const vec<3,T> &xyz, T _w )
: vec(xyz[0], xyz[1], xyz[2], _w) { }

template <typename T> vec<4, T>::vec( T _x, const vec<3,T> &yzw )
: vec(_x, yzw[0], yzw[1], yzw[2]) { }


template <typename T>
T& vec<4, T>::operator[]( size_t i )
{
    // return *(&x + i);
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
    // return *(&x + i);
    switch (i)
    {
        default: case 0: return x;
                 case 1: return y;
                 case 2: return z;
                 case 3: return w;
    }
}
