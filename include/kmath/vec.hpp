#pragma once

#include <stddef.h>
#include <stdint.h>
#include <algorithm>

template <size_t N, typename T>
struct vec;


#define vec_OPERATOR_VEC_VEC(op) \
template <size_t N, typename T> \
vec<N, T> operator op( const vec<N, T> &L, const vec<N, T> &R ) \
{ \
    vec<N, T> v;   for (size_t i=0; i<N; i++)     v[i] = L[i] op R[i]; \
    return v; \
}



#define vec_OPEQ_VEC_VEC(op) \
template <size_t N, typename T> \
vec<N, T> &operator op( vec<N, T> &L, const vec<N, T> &R ) \
{ \
    for (size_t i=0; i<N; i++) L[i] op R[i]; \
    return L; \
}


#define vec_OPERATOR_SCALAR_VEC(op) \
template <size_t N, typename T> \
vec<N, T> operator op( T L, const vec<N, T> &R ) \
{ \
    vec<N, T> v;   for (size_t i=0; i<N; i++)     v[i] = L op R[i]; \
    return v; \
}

#define vec_OPERATOR_VEC_SCALAR(op) \
template <size_t N, typename T> \
vec<N, T> operator op( const vec<N, T> &L, T R ) \
{ \
    vec<N, T> v;   for (size_t i=0; i<N; i++)     v[i] = L[i] op R; \
    return v; \
}


#define vec_OPERATOR_CONSTRUCT(sz) \
template <typename U> \
vec( const vec<sz, U> &R ) \
{ \
    for (size_t i=0; i<sz; i++)     (*this)[i] = T(R[i]); \
}


#define vec_OPERATOR_ASSIGN(sz)\
template <typename U>\
vec operator=( const vec<sz, U> &R )\
{\
    for (size_t i=0; i<sz; i++)     (*this)[i] = T(R[i]);\
    return *this;\
}\


vec_OPERATOR_VEC_VEC(+)
vec_OPERATOR_VEC_VEC(-)
vec_OPERATOR_VEC_VEC(*)
vec_OPERATOR_VEC_VEC(/)

vec_OPEQ_VEC_VEC(+=)
vec_OPEQ_VEC_VEC(-=)
vec_OPEQ_VEC_VEC(*=)
vec_OPEQ_VEC_VEC(/=)


vec_OPERATOR_SCALAR_VEC(+)
vec_OPERATOR_SCALAR_VEC(-)
vec_OPERATOR_SCALAR_VEC(*)
vec_OPERATOR_SCALAR_VEC(/)

vec_OPERATOR_VEC_SCALAR(+)
vec_OPERATOR_VEC_SCALAR(-)
vec_OPERATOR_VEC_SCALAR(*)
vec_OPERATOR_VEC_SCALAR(/)





template <typename T>
struct vec<2, T>
{
    union { T x, r; };
    union { T y, g; };

    vec(): vec(0) {  };
    vec( T xy ): vec(xy, xy) {  };
    vec( T _x, T _y ): x(_x), y(_y) {  };

    T& operator[]( size_t );
    T const& operator[]( size_t ) const;

    vec_OPERATOR_CONSTRUCT(2)
    vec_OPERATOR_ASSIGN(2)
};




template <typename T>
struct vec<3, T>
{
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };

    vec(): vec(0) {  };
    vec( T xx ): vec(xx, xx, xx) {  };
    vec( T _x, T _y, T _z ): x(_x), y(_y), z(_z) {  };

    vec( const vec<2,T>&, T );
    vec( T, const vec<2,T>& );

    T& operator[]( size_t );
    T const& operator[]( size_t ) const;

    vec_OPERATOR_CONSTRUCT(3)
    vec_OPERATOR_ASSIGN(3)
};


template <typename T>
struct vec<4, T>
{
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    union { T w, a; };

    vec(): vec(0) {  };
    vec( T xx ): vec(xx, xx, xx, xx) {  };
    vec( T _x, T _y, T _z, T _w ): x(_x), y(_y), z(_z), w(_w) {  };

    vec( const vec<2,T>&, T, T );
    vec( T, T, const vec<2,T>& );
    vec( const vec<2,T>&, const vec<2,T>& );
    vec( const vec<3,T>&, T );
    vec( T, const vec<3,T>& );

    T& operator[]( size_t );
    T const& operator[]( size_t ) const;

    template <typename U>
    vec operator=( const vec<3, U> &R )
    {
        for (size_t i=0; i<3; i++) (*this)[i] = T(R[i]);
        return *this;
    }
    // template <typename SZ, typename U>
    // vec operator=( const vec<SZ, U> &R )
    // {
    //     for (size_t i=0; i<SZ; i++) (*this)[i] = T(R[i]);
    //     return *this;
    // }

    vec_OPERATOR_CONSTRUCT(4)
    vec_OPERATOR_ASSIGN(4)
};


using vec2    = vec<2, float>;
using vec3    = vec<3, float>;
using vec4    = vec<4, float>;
using hvec2   = vec<2, _Float16>;
using hvec3   = vec<3, _Float16>;
using hvec4   = vec<4, _Float16>;
using dvec2   = vec<2, double>;
using dvec3   = vec<3, double>;
using dvec4   = vec<4, double>;

using ivec2   = vec<2, int32_t>;
using ivec3   = vec<3, int32_t>;
using ivec4   = vec<4, int32_t>;
using i8vec2  = vec<2, int8_t>;
using i8vec3  = vec<3, int8_t>;
using i8vec4  = vec<4, int8_t>;
using i16vec2 = vec<2, int16_t>;
using i16vec3 = vec<3, int16_t>;
using i16vec4 = vec<4, int16_t>;

using uvec2   = vec<2, uint32_t>;
using uvec3   = vec<3, uint32_t>;
using uvec4   = vec<4, uint32_t>;
using u8vec2  = vec<2, uint8_t>;
using u8vec3  = vec<3, uint8_t>;
using u8vec4  = vec<4, uint8_t>;
using u16vec2 = vec<2, uint16_t>;
using u16vec3 = vec<3, uint16_t>;
using u16vec4 = vec<4, uint16_t>;
using u32vec2 = vec<2, uint32_t>;
using u32vec3 = vec<3, uint32_t>;
using u32vec4 = vec<4, uint32_t>;
using u64vec2 = vec<2, uint64_t>;
using u64vec3 = vec<3, uint64_t>;
using u64vec4 = vec<4, uint64_t>;


#include "vec.inl"
