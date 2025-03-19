#pragma once

#include <algorithm>


template <typename T>
constexpr const T&
std::min( const T &A, const T &B )
{
    return (A<B) ? A : B;
}



template <typename T>
constexpr const T&
std::max( const T &A, const T &B )
{
    return (A>B) ? A : B;
}



template <typename T>
constexpr const T&
std::clamp( const T &X, const T &lo, const T &hi )
{
    return std::max(lo, std::min(X, hi));
}


