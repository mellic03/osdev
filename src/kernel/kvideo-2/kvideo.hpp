// #pragma once

// #include <stddef.h>
// #include <stdint.h>
// #include <kmath/vec.hpp>
// #include "texture.hpp"


// template <typename T>
// struct kframebuffer
// {
//     int w;
//     int h;
//     T *buf;

//     kframebuffer()
//     :   kframebuffer(0, 0, nullptr) {  };

//     kframebuffer( int _w, int _h, T *b )
//     :    w(_w), h(_h), buf(b) {  };

//     kframebuffer( int _w, int _h, T *b, bool )
//     :    w(_w), h(_h), buf(b) {  };

//     inline constexpr
//     T *operator[](int i)
//     {
//         return &(buf[w*i]);
//     };
    
//     inline constexpr
//     const T *operator[](int i) const
//     {
//         return &(buf[w*i]);
//     };

// };


// struct limine_mp_info;
// extern "C" { void kvideo_main( limine_mp_info* ); }

// namespace kvideo
// {
//     extern int    W;
//     extern int    H;
//     extern int    pitch;
//     extern size_t BPP;
//     extern size_t stride;
//     extern size_t nbytes;

//     extern uint8_t* volatile frontbuffer;
//     extern uint8_t* volatile backbuffer;

//     void initFrontbuffer( uintptr_t fb_res );
//     void initBackbuffer( uintptr_t fb_res );
//     void swapBuffers();

//     void fill( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
//     void fill( const vec3& );
//     void fill( const vec4& );

//     // void blit( ivec2 dst, ivec2 src, ivec2 sp, kframebuffer<vec4> &dstbuf,
//     //            const kframebuffer<vec4> &srcbuf );

//     // void blit( ivec2 dst, ivec2 src, ivec2 sp, const vec4 *buf, int bufw );
//     // void blit( ivec2 dst, ivec2 src, ivec2 sp, const uint32_t *buf, int bufw );
// }

