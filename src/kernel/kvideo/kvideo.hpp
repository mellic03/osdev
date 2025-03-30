#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kmath/vec.hpp>


template <typename T>
struct kframebuffer
{
    int w;
    int h;
    T *buf;

    kframebuffer()
    :   kframebuffer(0, 0, nullptr) {  };

    kframebuffer( int _w, int _h, T *b )
    :    w(_w), h(_h), buf(b) {  };

    inline constexpr
    T *operator[](int i)
    {
        return &(buf[w*i]);
    };
    
    inline constexpr
    const T *operator[](int i) const
    {
        return &(buf[w*i]);
    };

};


namespace kvideo
{
    extern kframebuffer<uint32_t> frontbuffer;
    extern kframebuffer<uint32_t> backbuffer;


    void init( uintptr_t fb_res );
    void swapBuffers();

    void fill( uint32_t );
    void fill( const vec3& );
    void fill( const vec4& );

    void blit( ivec2 dst, ivec2 src, ivec2 sp, kframebuffer<vec4> &dstbuf,
               const kframebuffer<vec4> &srcbuf );

    void blit( ivec2 dst, ivec2 src, ivec2 sp,
               const kframebuffer<vec4> &buf );
}

