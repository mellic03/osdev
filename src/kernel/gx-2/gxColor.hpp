#pragma once
#include <kmath/vec.hpp>

inline uint32_t gx_vec4_to_u32( const vec4 &rgba )
{
    uint32_t a = uint32_t(255.0f * rgba.a);
    uint32_t r = uint32_t(255.0f * rgba.r);
    uint32_t g = uint32_t(255.0f * rgba.g);
    uint32_t b = uint32_t(255.0f * rgba.b);
    return (a<<24) + (r<<16) + (g<<8) + (b<<0);
}

inline vec4 gx_u32_to_vec4( uint32_t rgba )
{
    vec4 C = vec4(
        uint8_t(rgba << 16),
        uint8_t(rgba << 8),
        uint8_t(rgba << 0),
        uint8_t(rgba << 24)
    );

    return C;
}
