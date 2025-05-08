// #pragma once

// #include <stddef.h>
// #include <stdint.h>

// #include <kfs/bmp.hpp>
// #include <kmath/vec.hpp>
// #include "texture.hpp"


// namespace idk
// {
//     struct Bounds
//     {
//         ivec2 corner;
//         ivec2 extents;
//     };
// }


// namespace idk
// {
//     class FontBuffer;
// }



// class idk::FontBuffer
// {
// private:

// public:
//     int W, H, SZ;
//     vec2 glyph_scale;
//     uint32_t m_tex;

//     FontBuffer();
//     FontBuffer( BMP_header* );

//     ivec2 getGlyphExtents();
//     ivec2 getGlyphCorner( char c );

//     // void *data()
//     // {
//     //     return m_data;
//     // }

//     // const void *data() const
//     // {
//     //     return m_data;
//     // }

//     // vec4 *operator[] ( int i )
//     // {
//     //     return &(m_data[W*i]);
//     // }

//     // const vec4 *operator[] ( int i ) const
//     // {
//     //     return &(m_data[W*i]);
//     // }

// };

