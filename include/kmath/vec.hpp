#pragma once

#include "vec_n.hpp"




#define TVEC_OPERATOR_VEC_VEC(op) \
template <typename T, size_t N> \
tvec<T, N> operator op( const tvec<T, N> &L, const tvec<T, N> &R ) \
{ \
    tvec<T, N> v;   for (int i=0; i<N; i++)     v[i] = L[i] op R[i]; \
    return v; \
}



#define TVEC_OPEQ_VEC_VEC(op) \
template <typename T, size_t N> \
tvec<T, N> &operator op( tvec<T, N> &L, const tvec<T, N> &R ) \
{ \
    for (int i=0; i<N; i++) L[i] op R[i]; \
    return L; \
}


#define TVEC_OPERATOR_SCALAR_VEC(op) \
template <typename T, size_t N> \
tvec<T, N> operator op( T L, const tvec<T, N> &R ) \
{ \
    tvec<T, N> v;   for (int i=0; i<N; i++)     v[i] = L op R[i]; \
    return v; \
}

#define TVEC_OPERATOR_VEC_SCALAR(op) \
template <typename T, size_t N> \
tvec<T, N> operator op( const tvec<T, N> &L, T R ) \
{ \
    tvec<T, N> v;   for (int i=0; i<N; i++)     v[i] = L[i] op R; \
    return v; \
}


#define TVEC_OPERATOR_ARRAY() \
T &operator[]( int i ) { return data[i]; } \
const T &operator[]( int i ) const { return data[i]; } \


#define TVEC_OPERATOR_CONSTRUCT(sz) \
template <typename U> \
tvec( const tvec<U, sz> &R ) \
{ \
    for (int i=0; i<sz; i++)     data[i] = T(R[i]); \
}


#define TVEC_OPERATOR_ASSIGN(sz) \
template <typename U> \
tvec operator=( const tvec<U, sz> &R ) \
{ \
    for (int i=0; i<sz; i++)     data[i] = T(R[i]); \
    return *this; \
}



TVEC_OPERATOR_VEC_VEC(+)
TVEC_OPERATOR_VEC_VEC(-)
TVEC_OPERATOR_VEC_VEC(*)
TVEC_OPERATOR_VEC_VEC(/)

TVEC_OPEQ_VEC_VEC(+=)
TVEC_OPEQ_VEC_VEC(-=)
TVEC_OPEQ_VEC_VEC(*=)
TVEC_OPEQ_VEC_VEC(/=)


TVEC_OPERATOR_SCALAR_VEC(+)
TVEC_OPERATOR_SCALAR_VEC(-)
TVEC_OPERATOR_SCALAR_VEC(*)
TVEC_OPERATOR_SCALAR_VEC(/)

TVEC_OPERATOR_VEC_SCALAR(+)
TVEC_OPERATOR_VEC_SCALAR(-)
TVEC_OPERATOR_VEC_SCALAR(*)
TVEC_OPERATOR_VEC_SCALAR(/)




template <typename T>
struct tvec<T, 2>
{
    union {
        T data[2];
        struct {
            union { T x, r; };
            union { T y, g; };
        };
    };

    tvec(): tvec(0) {  };
    tvec( T xy ): tvec(xy, xy) {  };
    tvec( T _x, T _y ): x(_x), y(_y) {  };

    TVEC_OPERATOR_CONSTRUCT(2)
    TVEC_OPERATOR_ASSIGN(2)
    TVEC_OPERATOR_ARRAY()
};


template <typename T>
struct tvec<T, 3>
{
    union {
        T data[3];
        struct {
            union { T x, r; };
            union { T y, g; };
            union { T z, b; };
        };
    };

    tvec(): tvec(0) {  };
    tvec( T xx ): tvec(xx, xx, xx) {  };
    tvec( T _x, T _y, T _z ): x(_x), y(_y), z(_z) {  };

    TVEC_OPERATOR_CONSTRUCT(3)
    TVEC_OPERATOR_ASSIGN(3)
    TVEC_OPERATOR_ARRAY()
};


template <typename T>
struct tvec<T, 4>
{
    union {
        T data[4];
        struct {
            union { T x, r; };
            union { T y, g; };
            union { T z, b; };
            union { T w, a; };
        };
    };

    tvec(): tvec(0) {  };
    tvec( T xx ): tvec(xx, xx, xx, xx) {  };
    tvec( T _x, T _y, T _z, T _w ): x(_x), y(_y), z(_z), w(_w) {  };

    TVEC_OPERATOR_CONSTRUCT(4)
    TVEC_OPERATOR_ASSIGN(4)
    TVEC_OPERATOR_ARRAY()
};




template <typename T>
using tvec2 = tvec<T, 2>;


template <typename T>
using tvec3 = tvec<T, 3>;


template <typename T>
using tvec4 = tvec<T, 4>;



// template <typename T>
// struct tvec_4: public tvec<T, 4>
// {
//     union { T &x, &r; };
//     union { T &y, &g; };
//     union { T &z, &b; };
//     union { T &w, &a; };

//     tvec_4(): x(this->data[0]), y(this->data[1]), z(this->data[2]), w(this->data[3]) {  };
//     tvec_4( T xx ): tvec_4(xx, xx, xx, xx) {  };
//     tvec_4( T _x, T _y, T _z, T _w ): tvec_4()
//     {
//         x = _x;
//         y = _y;
//         z = _z;
//         w = _w;
//     };


// };








// template <typename T>
// struct tvec2;

// template <typename T>
// struct tvec3;

// template <typename T>
// struct tvec4;


// template <typename T>
// struct tvec2
// {
//     union { T x, r; };
//     union { T y, g; };

//     tvec2(): x(0), y(0) {  };
//     tvec2( T _xy ): x(_xy), y(_xy) {  };
//     tvec2( T _x, T _y ): x(_x), y(_y) {  };

//     template <typename U>
//     tvec2( const tvec2<U> &rhs )
//     :   tvec2(T(rhs.x), T(rhs.y)) {  };

//     template <typename U>
//     tvec2( const tvec3<U> &rhs )
//     :   tvec2(T(rhs.x), T(rhs.y)) {  };


//     tvec2 operator+( const tvec2 &rhs )
//     {
//         tvec2 v;
//         v.x += rhs.x;
//         v.y += rhs.y;
//         return v;
//     }

//     tvec2 operator-( const tvec2 &rhs )
//     {
//         tvec2 v;
//         v.x -= rhs.x;
//         v.y -= rhs.y;
//         return v;
//     }

//     tvec2 operator*( const tvec2 &rhs )
//     {
//         tvec2 v;
//         v.x *= rhs.x;
//         v.y *= rhs.y;
//         return v;
//     }

//     tvec2 &operator+=( const tvec2 &rhs )
//     {
//         x += rhs.x;
//         y += rhs.y;
//         return *this;
//     }

//     tvec2 &operator*=( const T &rhs )
//     {
//         x *= rhs;
//         y *= rhs;
//         return *this;
//     }

//     tvec2 &operator/=( const T &rhs )
//     {
//         x /= rhs;
//         y /= rhs;
//         return *this;
//     }

//     bool operator==( const tvec2 &rhs ) const
//     {
//         return x == rhs.x && y == rhs.y;
//     }

//     bool operator==( const T &rhs ) const
//     {
//         return (x == rhs) && (y == rhs);
//     }


//     template <typename U>
//     tvec2 operator=( const tvec2<U> &rhs )
//     {
//         x = T(rhs.x);
//         y = T(rhs.y);
//         return *this;
//     }
// };



// template <typename T>
// struct tvec3
// {
//     union { T x, r; };
//     union { T y, g; };
//     union { T z, b; };

//     tvec3(): x(0), y(0), z(0) {  };
//     tvec3( T _xyz ): x(_xyz), y(_xyz), z(_xyz) {  };
//     tvec3( T _x, T _y, T _z ): x(_x), y(_y), z(_z) {  };

//     tvec3( tvec2<T> _xy, T _z ): x(_xy.x), y(_xy.y), z(_z) {  };
//     tvec3( T _x, tvec2<T> _yz ): x(_x), y(_yz.x), z(_yz.y) {  };
// };


// template <typename T>
// struct tvec4
// {
//     union { T x, r; };
//     union { T y, g; };
//     union { T z, b; };
//     union { T w, a; };

//     tvec4(): x(0), y(0), z(0), w(0) {  };
//     tvec4( T _xyzw ): x(_xyzw), y(_xyzw), z(_xyzw), w(_xyzw) {  };
//     tvec4( T _x, T _y, T _z, T _w ): x(_x), y(_y), z(_z), w(_w) {  };


//     tvec4 operator+( const tvec4 &rhs )
//     {
//         tvec4 v;
//         v.x += rhs.x;
//         v.y += rhs.y;
//         v.z += rhs.z;
//         v.w += rhs.w;
//         return v;
//     }

//     tvec4 operator-( const tvec4 &rhs )
//     {
//         tvec4 v;
//         v.x -= rhs.x;
//         v.y -= rhs.y;
//         v.z -= rhs.z;
//         v.w -= rhs.w;
//         return v;
//     }

//     tvec4 operator*( const tvec4 &rhs )
//     {
//         tvec4 v;
//         v.x *= rhs.x;
//         v.y *= rhs.y;
//         v.z *= rhs.z;
//         v.w *= rhs.w;
//         return v;
//     }


//     tvec4 &operator/=( const T &rhs )
//     {
//         x /= rhs;
//         y /= rhs;
//         z /= rhs;
//         w /= rhs;
//         return *this;
//     }

// };


// template <typename T>
// tvec2<T> operator*( T s, const tvec2<T> &v )
// {
//     tvec2<T> sv;
//     sv.x *= s;
//     sv.y *= s;
//     return sv;
// }


// template <typename T>
// tvec4<T> operator*( T s, const tvec4<T> &v )
// {
//     tvec4<T> sv;
//     sv.x *= s;
//     sv.y *= s;
//     sv.z *= s;
//     sv.w *= s;
//     return sv;
// }



using vec2  = tvec2<float>;
using vec3  = tvec3<float>;
using vec4  = tvec4<float>;
using dvec2 = tvec2<double>;
using dvec3 = tvec3<double>;
using dvec4 = tvec4<double>;


using ivec2 = tvec2<int32_t>;
using ivec3 = tvec3<int32_t>;
using ivec4 = tvec4<int32_t>;

using i8vec2 = tvec2<int8_t>;
using i8vec3 = tvec3<int8_t>;
using i8vec4 = tvec4<int8_t>;

using i16vec2 = tvec2<int16_t>;
using i16vec3 = tvec3<int16_t>;
using i16vec4 = tvec4<int16_t>;


using uvec2 = tvec2<uint32_t>;
using uvec3 = tvec3<uint32_t>;
using uvec4 = tvec4<uint32_t>;

using u8vec2 = tvec2<uint8_t>;
using u8vec3 = tvec3<uint8_t>;
using u8vec4 = tvec4<uint8_t>;

using u16vec2 = tvec2<uint16_t>;
using u16vec3 = tvec3<uint16_t>;
using u16vec4 = tvec4<uint16_t>;

using u32vec2 = tvec2<uint32_t>;
using u32vec3 = tvec3<uint32_t>;
using u32vec4 = tvec4<uint32_t>;

using u64vec2 = tvec2<uint64_t>;
using u64vec3 = tvec3<uint64_t>;
using u64vec4 = tvec4<uint64_t>;



