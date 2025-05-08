#pragma once
#include <kmath/vec.hpp>
#include <type_traits>



template <typename T>
__attribute__((always_inline))
inline vec<4, T> &gx_mix( vec<4, T> &dst, const vec<4, T> &src )
{
    if constexpr (std::is_integral_v<T>)
    {
        // dst[0] = src[0] + (dst[0] * (255 - src.a));
        // dst[1] = src[1] + (dst[1] * (255 - src.a));
        // dst[2] = src[2] + (dst[2] * (255 - src.a));
        // dst[3] = src[3] + (dst[3] * (255 - src.a));
        dst[0] = ((255-src.a)*dst[0] + src.a*src[0]) / 255;
        dst[1] = ((255-src.a)*dst[1] + src.a*src[1]) / 255;
        dst[2] = ((255-src.a)*dst[2] + src.a*src[2]) / 255;
        dst[3] = ((255-src.a)*dst[3] + src.a*src[3]) / 255;
    }

    else
    {
        dst = src + (dst * (1.0f - src.a));
    }

    return dst;
}


// __attribute__((always_inline))
// inline void gx_mix( u8vec4 &dst, const u8vec4 &src )
// {
//     dst[0] = ((255-src.a)*dst[0] + src.a*src[0]) / 255;
//     dst[1] = ((255-src.a)*dst[1] + src.a*src[1]) / 255;
//     dst[2] = ((255-src.a)*dst[2] + src.a*src[2]) / 255;
//     dst[3] = ((255-src.a)*dst[3] + src.a*src[3]) / 255;
// }

// // __attribute__((always_inline))
// // inline void gx_mix( u8vec4 &dst, const uint8_t &src )
// // {
// //     gx_mix(dst, u8vec4(src));
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( u8vec4 &dst, const u8vec3 &src )
// // {
// //     gx_mix(dst, u8vec4(src, 255));
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( u8vec4 &dst, const float &src )
// // {
// //     gx_mix(dst, u8vec4(255.0f * src));
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( u8vec4 &dst, const vec4 &src )
// // {
// //     gx_mix(dst, u8vec4(255.0f * src));
// // }




// // __attribute__((always_inline))
// // inline void gx_mix( uint8_t &dst, const float &src )
// // {
// //     dst = uint8_t(255.0f * src);
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( uint8_t &dst, const vec4 &src )
// // {
// //     gx_mix(dst, src.r);
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( uint8_t &dst, const u8vec4 &src )
// // {
// //     gx_mix(dst, float(src.r) / 255.0f);
// // }




// // __attribute__((always_inline))
// // inline void gx_mix( float &dst, const float &src )
// // {
// //     dst = src;
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( float &dst, const vec4 &src )
// // {
// //     gx_mix(dst, src.r);
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( float &dst, const u8vec4 &src )
// // {
// //     gx_mix(dst, float(src.r) / 255.0f);
// // }







// // __attribute__((always_inline))
// // inline void gx_mix( vec4 &dst, const vec4 &src )
// // {
// //     dst = (1.0f - src.a)*dst + src.a*src;
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( vec4 &dst, const vec3 &src )
// // {
// //     gx_mix(dst, vec4(src, 1.0f));
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( vec4 &dst, const float &src )
// // {
// //     gx_mix(dst, vec4(src));
// // }

// // __attribute__((always_inline))
// // inline void gx_mix( vec4 &dst, const u8vec4 &src )
// // {
// //     gx_mix(dst, vec4(src) / 255.0f);
// // }



// __attribute__((always_inline))
// inline void gx_mix_internal( uint8_t &dst, const uint8_t &src )
// {
//     dst = src;
// }

// __attribute__((always_inline))
// inline void gx_mix_internal( u8vec3 &dst, const u8vec3 &src )
// {
//     dst = src;
// }

// __attribute__((always_inline))
// inline void gx_mix_internal( u8vec4 &dst, const u8vec4 &src )
// {
//     vec4 A = vec4(dst) / 255.0f;
//     vec4 B = vec4(src) / 255.0f;
//     A = (1.0f - B.a)*A + B.a*B;
//     dst = u8vec4(255.0f * A);
// }



// __attribute__((always_inline))
// inline void gx_mix_internal( float &dst, const float &src )
// {
//     dst = src;
// }

// __attribute__((always_inline))
// inline void gx_mix_internal( vec3 &dst, const vec3 &src )
// {
//     dst = src;
// }

// __attribute__((always_inline))
// inline void gx_mix_internal( vec4 &dst, const vec4 &src )
// {
//     // dst = (1.0f - src.a)*dst + src.a*src;
//     dst = src + (dst * (1.0f - src.a));
// }


// template <typename dst_type, typename src_type>
// __attribute__((always_inline))
// inline void gx_mix( dst_type &dst, const src_type &src )
// {
//     dst_type temp;
//     gx_assign(temp, src);
//     gx_mix_internal(dst, temp);
// }


