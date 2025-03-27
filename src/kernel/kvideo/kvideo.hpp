#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kmath/vec.hpp>


extern "C"
{
    struct limine_framebuffer_response;
}


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


    void init( limine_framebuffer_response* );
    void swapBuffers();


    void blit( ivec2 dst, ivec2 src, ivec2 sp, kframebuffer<vec4> &dstbuf,
               const kframebuffer<vec4> &srcbuf );

    void blit( ivec2 dst, ivec2 src, ivec2 sp,
               const kframebuffer<vec4> &buf );
}

