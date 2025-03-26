#pragma once

#include <stddef.h>
#include <stdint.h>


// namespace idk
// {
    // template <typename T, size_t N>
    // struct tvec;

    template <typename T>
    struct tvec2;

    template <typename T>
    struct tvec3;

    template <typename T>
    struct tvec4;

// }



template <typename T>
struct tvec2
{
    union { T x, r; };
    union { T y, g; };

    tvec2(): x(0), y(0) {  };
    tvec2( T _xy ): x(_xy), y(_xy) {  };
    tvec2( T _x, T _y ): x(_x), y(_y) {  };

    template <typename U>
    tvec2( const tvec2<U> &rhs )
    :   tvec2(T(rhs.x), T(rhs.y)) {  };

    template <typename U>
    tvec2( const tvec3<U> &rhs )
    :   tvec2(T(rhs.x), T(rhs.y)) {  };


    tvec2 operator+( const tvec2 &rhs )
    {
        tvec2 v;
        v.x += rhs.x;
        v.y += rhs.y;
        return v;
    }

    tvec2 operator-( const tvec2 &rhs )
    {
        tvec2 v;
        v.x -= rhs.x;
        v.y -= rhs.y;
        return v;
    }

    tvec2 operator*( const tvec2 &rhs )
    {
        tvec2 v;
        v.x *= rhs.x;
        v.y *= rhs.y;
        return v;
    }

    tvec2 &operator+=( const tvec2 &rhs )
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    tvec2 &operator*=( const T &rhs )
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    tvec2 &operator/=( const T &rhs )
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    bool operator==( const tvec2 &rhs ) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool operator==( const T &rhs ) const
    {
        return (x == rhs) && (y == rhs);
    }


    template <typename U>
    tvec2 operator=( const tvec2<U> &rhs )
    {
        x = T(rhs.x);
        y = T(rhs.y);
        return *this;
    }
};


template <typename T>
struct tvec3
{
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };

    tvec3(): x(0), y(0), z(0) {  };
    tvec3( T _xyz ): x(_xyz), y(_xyz), z(_xyz) {  };
    tvec3( T _x, T _y, T _z ): x(_x), y(_y), z(_z) {  };

    tvec3( tvec2<T> _xy, T _z ): x(_xy.x), y(_xy.y), z(_z) {  };
    tvec3( T _x, tvec2<T> _yz ): x(_x), y(_yz.x), z(_yz.y) {  };
};


template <typename T>
struct tvec4
{
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    union { T w, a; };

    tvec4(): x(0), y(0), z(0), w(0) {  };
    tvec4( T _xyzw ): x(_xyzw), y(_xyzw), z(_xyzw), w(_xyzw) {  };
    tvec4( T _x, T _y, T _z, T _w ): x(_x), y(_y), z(_z), w(_w) {  };


    tvec4 operator+( const tvec4 &rhs )
    {
        tvec4 v;
        v.x += rhs.x;
        v.y += rhs.y;
        v.z += rhs.z;
        v.w += rhs.w;
        return v;
    }

    tvec4 operator-( const tvec4 &rhs )
    {
        tvec4 v;
        v.x -= rhs.x;
        v.y -= rhs.y;
        v.z -= rhs.z;
        v.w -= rhs.w;
        return v;
    }

    tvec4 operator*( const tvec4 &rhs )
    {
        tvec4 v;
        v.x *= rhs.x;
        v.y *= rhs.y;
        v.z *= rhs.z;
        v.w *= rhs.w;
        return v;
    }


    tvec4 &operator/=( const T &rhs )
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

};


template <typename T>
tvec2<T> operator*( T s, const tvec2<T> &v )
{
    tvec2<T> sv;
    sv.x *= s;
    sv.y *= s;
    return sv;
}


template <typename T>
tvec4<T> operator*( T s, const tvec4<T> &v )
{
    tvec4<T> sv;
    sv.x *= s;
    sv.y *= s;
    sv.z *= s;
    sv.w *= s;
    return sv;
}


// namespace idk
// {
    // #ifdef __libk_sse
        using vec2  = tvec2<float>;
        using vec3  = tvec3<float>;
        using vec4  = tvec4<float>;
        using dvec2 = tvec2<double>;
        using dvec3 = tvec3<double>;
        using dvec4 = tvec4<double>;
    // #else
    //     using vec2  = tvec2<uint32_t>;
    //     using vec3  = tvec3<uint32_t>;
    //     using vec4  = tvec4<uint32_t>;
    //     using dvec2 = tvec2<uint64_t>;
    //     using dvec3 = tvec3<uint64_t>;
    //     using dvec4 = tvec4<uint64_t>;
    // #endif


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

// }



