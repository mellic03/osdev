#pragma once
#include <stddef.h>
#include <stdint.h>
#include <algorithm>

template <size_t N, typename T>
struct vec;


#define vec_OPERATOR_VEC_VEC(op)\
template <size_t N, typename T, typename U>\
vec<N, T> operator op( const vec<N, T> &L, const vec<N, U> &R )\
{\
    vec<N, T> res;\
    for (size_t i=0; i<N; i++) { res[i] = L[i] op static_cast<T>(R[i]); }\
    return res;\
}



#define vec_OPEQ_VEC_VEC(op)\
template <size_t N, typename T, typename U>\
vec<N, T> &operator op( vec<N, T> &L, const vec<N, U> &R )\
{\
    for (size_t i=0; i<N; i++) L[i] op static_cast<T>(R[i]);\
    return L;\
}\
template <size_t N, typename T>\
vec<N, T> &operator op( vec<N, T> &L, T R )\
{\
    for (size_t i=0; i<N; i++) L[i] op R;\
    return L;\
}


#define vec_OPERATOR_SCALAR_VEC(op)\
template <size_t N, typename T>\
vec<N, T> operator op( T L, const vec<N, T> &R )\
{\
    vec<N, T> v;   for (size_t i=0; i<N; i++)     v[i] = L op R[i];\
    return v;\
}

#define vec_OPERATOR_VEC_SCALAR(op)\
template <size_t N, typename T>\
vec<N, T> operator op( const vec<N, T> &L, T R )\
{\
    vec<N, T> v;   for (size_t i=0; i<N; i++)     v[i] = L[i] op R;\
    return v;\
}


#define vec_OPERATOR_CONSTRUCT(sz)\
template <typename U>\
vec( const vec<sz, U> &R )\
{\
    for (size_t i=0; i<sz; i++)     (*this)[i] = T(R[i]);\
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
struct vec<1, T>
{
    union { T x, r; };

    vec(): vec(0) {  };
    vec( T _x ): vec(x) {  };

    T& operator[]( size_t );
    T const& operator[]( size_t ) const;

    bool operator==( const vec<1, T> &rhs ) const
    {
        return x==rhs.x;
    }

    vec_OPERATOR_CONSTRUCT(1)
    vec_OPERATOR_ASSIGN(1)

} __attribute__((aligned(1*sizeof(T))));



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

    bool operator==( const vec<2, T> &rhs ) const
    {
        return x==rhs.x && y==rhs.y;
    }

    vec_OPERATOR_CONSTRUCT(2)
    vec_OPERATOR_ASSIGN(2)

} __attribute__((aligned(2*sizeof(T))));




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

    bool operator==( const vec<3, T> &rhs ) const
    {
        return x==rhs.x && y==rhs.y && z==rhs.z;
    }

    vec_OPERATOR_CONSTRUCT(3)
    vec_OPERATOR_ASSIGN(3)

} __attribute__((aligned(4*sizeof(T))));


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

    bool operator==( const vec<4, T> &rhs ) const
    {
        return x==rhs.x && y==rhs.y && z==rhs.z && w==rhs.w;
    }

    vec_OPERATOR_CONSTRUCT(4)
    vec_OPERATOR_ASSIGN(4)

} __attribute__((aligned(4*sizeof(T))));



using vec1    = vec<1, float>;
using vec2    = vec<2, float>;
using vec3    = vec<3, float>;
using vec4    = vec<4, float>;
// using hvec1   = vec<1, _Float16>;
// using hvec2   = vec<2, _Float16>;
// using hvec3   = vec<3, _Float16>;
// using hvec4   = vec<4, _Float16>;
using dvec1   = vec<1, double>;
using dvec2   = vec<2, double>;
using dvec3   = vec<3, double>;
using dvec4   = vec<4, double>;

using ivec1   = vec<1, int32_t>;
using ivec2   = vec<2, int32_t>;
using ivec3   = vec<3, int32_t>;
using ivec4   = vec<4, int32_t>;
using i8vec1  = vec<1, int8_t>;
using i8vec2  = vec<2, int8_t>;
using i8vec3  = vec<3, int8_t>;
using i8vec4  = vec<4, int8_t>;
using i16vec2 = vec<2, int16_t>;
using i16vec3 = vec<3, int16_t>;
using i16vec4 = vec<4, int16_t>;

using uvec1   = vec<1, uint32_t>;
using uvec2   = vec<2, uint32_t>;
using uvec3   = vec<3, uint32_t>;
using uvec4   = vec<4, uint32_t>;
using u8vec1  = vec<1, uint8_t>;
using u8vec2  = vec<2, uint8_t>;
using u8vec3  = vec<3, uint8_t>;
using u8vec4  = vec<4, uint8_t>;
using u16vec1 = vec<1, uint16_t>;
using u16vec2 = vec<2, uint16_t>;
using u16vec3 = vec<3, uint16_t>;
using u16vec4 = vec<4, uint16_t>;
using u32vec1 = vec<1, uint32_t>;
using u32vec2 = vec<2, uint32_t>;
using u32vec3 = vec<3, uint32_t>;
using u32vec4 = vec<4, uint32_t>;
using u64vec1 = vec<1, uint64_t>;
using u64vec2 = vec<2, uint64_t>;
using u64vec3 = vec<3, uint64_t>;
using u64vec4 = vec<4, uint64_t>;


inline static uint32_t vec4_pack_argb( const vec4 &v )
{
    uint32_t a = uint32_t(255.0f * v.a) << 24;
    uint32_t r = uint32_t(255.0f * v.r) << 16;
    uint32_t g = uint32_t(255.0f * v.g) << 8;
    uint32_t b = uint32_t(255.0f * v.b) << 0;
    return a + r + g + b;
}



template <size_t N, typename T>
__attribute__((always_inline))
inline vec<N, T> vec_clamp( vec<N, T> v, T lo, T hi )
{
    for (size_t i=0; i<N; i++)
        v[i] = std::clamp(v[i], lo, hi);
    return v;
}


template <size_t N, typename T>
__attribute__((always_inline))
inline vec<N, T> vec_clamp( vec<N, T> v, const vec<N, T> &lo, const vec<N, T> &hi )
{
    for (size_t i=0; i<N; i++)
        v[i] = std::clamp(v[i], lo[i], hi[i]);
    return v;
}




template <size_t N, typename T>
__attribute__((always_inline))
inline vec<N, T> vec_mix( const vec<N, T> &X, const vec<N, T> &Y, float a )
{
    return (1.0f - a)*X + a*Y;
}



#include "vec.inl"
