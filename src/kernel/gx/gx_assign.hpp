#pragma once
#include <kmath/vec.hpp>


__attribute__((always_inline))
inline void gx_assign( uint8_t &dst, const uint8_t &src )
{
    dst = src;
}

__attribute__((always_inline))
inline void gx_assign( uint8_t &dst, const vec4 &src )
{
    dst = uint8_t(255.0f * src.r);
}

__attribute__((always_inline))
inline void gx_assign( uint8_t &dst, const u8vec4 &src )
{
    dst = src.r;
}




__attribute__((always_inline))
inline void gx_assign( u8vec3 &dst, const u8vec3 &src )
{
    dst = src;
}

__attribute__((always_inline))
inline void gx_assign( u8vec3 &dst, const uint8_t &src )
{
    dst = u8vec3(src);
}

__attribute__((always_inline))
inline void gx_assign( u8vec3 &dst, const u8vec4 &src )
{
    gx_assign(dst, u8vec3(src));
}

__attribute__((always_inline))
inline void gx_assign( u8vec3 &dst, const float &src )
{
    dst = u8vec3(uint8_t(255.0f * src));
}

__attribute__((always_inline))
inline void gx_assign( u8vec3 &dst, const vec4 &src )
{
    dst = u8vec3(255.0f * src);
}





__attribute__((always_inline))
inline void gx_assign( u8vec4 &dst, const u8vec4 &src )
{
    dst = src;
}

__attribute__((always_inline))
inline void gx_assign( u8vec4 &dst, const uint8_t &src )
{
    dst = u8vec4(src);
}

__attribute__((always_inline))
inline void gx_assign( u8vec4 &dst, const u8vec3 &src )
{
    gx_assign(dst, u8vec4(src, 255));
}

__attribute__((always_inline))
inline void gx_assign( u8vec4 &dst, const float &src )
{
    dst = u8vec4(uint8_t(255.0f * src));
}

__attribute__((always_inline))
inline void gx_assign( u8vec4 &dst, const vec4 &src )
{
    dst = u8vec4(255.0f * src);
}




__attribute__((always_inline))
inline void gx_assign( float &dst, const float &src )
{
    dst = src;
}

__attribute__((always_inline))
inline void gx_assign( float &dst, const vec4 &src )
{
    dst = src.r;
}

__attribute__((always_inline))
inline void gx_assign( float &dst, const u8vec4 &src )
{
    dst = float(src.r) / 255.0f;
}







__attribute__((always_inline))
inline void gx_assign( vec3 &dst, const vec4 &src )
{
    dst = vec3(src);
}

__attribute__((always_inline))
inline void gx_assign( vec3 &dst, const vec3 &src )
{
    dst = src;
}

__attribute__((always_inline))
inline void gx_assign( vec3 &dst, const float &src )
{
    dst = vec3(src);
}

__attribute__((always_inline))
inline void gx_assign( vec3 &dst, const u8vec4 &src )
{
    dst = vec3(src) / 255.0f;
}




__attribute__((always_inline))
inline void gx_assign( vec4 &dst, const vec4 &src )
{
    dst = src;
}

__attribute__((always_inline))
inline void gx_assign( vec4 &dst, const vec3 &src )
{
    dst = vec4(src, 1.0f);
}

__attribute__((always_inline))
inline void gx_assign( vec4 &dst, const float &src )
{
    dst = vec4(src);
}

__attribute__((always_inline))
inline void gx_assign( vec4 &dst, const uint8_t &src )
{
    dst = vec4(src) / 255.0f;
}

__attribute__((always_inline))
inline void gx_assign( vec4 &dst, const u8vec4 &src )
{
    dst = vec4(src) / 255.0f;
}

