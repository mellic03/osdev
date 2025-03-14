#pragma once

#include <stddef.h>
#include <stdint.h>


namespace idk
{
    template <typename T>
    struct tvec2;

    template <typename T>
    struct tvec3;

    template <typename T>
    struct tvec4;
}


template <typename T>
struct idk::tvec2
{
    union { T x, r; };
    union { T y, g; };

    tvec2(): x(0), y(0) {  };
    tvec2( T _xy ): x(_xy), y(_xy) {  };
    tvec2( T _x, T _y ): x(_x), y(_y) {  };
};


template <typename T>
struct idk::tvec3
{
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };

    tvec3(): x(0), y(0), z(0) {  };
    tvec3( T _xyz ): x(_xyz), y(_xyz), z(_xyz) {  };
    tvec3( T _x, T _y, T _z ): x(_x), y(_y), z(_z) {  };
};


template <typename T>
struct idk::tvec4
{
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    union { T w, a; };

    tvec4(): x(0), y(0), z(0), w(0) {  };
    tvec4( T _xyzw ): x(_xyzw), y(_xyzw), z(_xyzw), w(_xyzw) {  };
    tvec4( T _x, T _y, T _z, T _w ): x(_x), y(_y), z(_z), w(_w) {  };
};



namespace idk
{
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


}
