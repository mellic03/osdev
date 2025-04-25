// #include "font.hpp"
// #include <string.h>


// idk::FontBuffer::FontBuffer()
// {
//     W = 0;
//     H = 0;
//     SZ = 0;
//     glyph_scale = vec2(1.0f, 1.0f);
// }


// vec4 ree_vec4( uint32_t src )
// {
//     float a = float((src >> 24) & 0xFF) / 255.0f;
//     float r = float((src >> 16) & 0xFF) / 255.0f;
//     float g = float((src >> 8) & 0xFF)  / 255.0f;
//     float b = float((src & 0xFF))       / 255.0f;

//     return vec4(r, g, b, a);
// }


// #include <kmalloc.h>

// idk::FontBuffer::FontBuffer( BMP_header *header )
// :   glyph_scale(1.0f)
// {
//     auto &info = header->infoheader;


//     uint32_t *aligned = new uint32_t[info.width * info.height];
//     memcpy(aligned, (uint8_t*)header + header->offset, info.width*info.height*sizeof(uint32_t));
//     m_data = new vec4[info.width*info.height];

//     this->W  = info.width;
//     this->H  = info.height;
//     this->SZ = W*H;

//     for (int i=0; i<W*H; i++)
//     {
//         m_data[i] = ree_vec4(aligned[i]);
//     }

//     for (int i=0; i<H/2; i++)
//     {
//         for (int j=0; j<W; j++)
//         {
//             vec4 temp = (*this)[H-1-i][j];
//             (*this)[H-1-i][j] = (*this)[i][j];
//             (*this)[i][j] = temp;
//         }
//     }
// }



// ivec2
// idk::FontBuffer::getGlyphExtents()
// {
//     static constexpr int GLYPH_ROWS = 6;
//     static constexpr int GLYPH_COLS = 16;
//     const int GLYPH_W = this->W / GLYPH_COLS;
//     const int GLYPH_H = this->H / GLYPH_ROWS;

//     vec2 sp = glyph_scale * vec2(GLYPH_W, GLYPH_H);

//     return ivec2(sp);
// }


// ivec2
// idk::FontBuffer::getGlyphCorner( char c )
// {
//     static constexpr char cmin = ' ';
//     static constexpr char cmax = '~';

//     if ((cmin<=c && c<=cmax) == false)
//     {
//         return ivec2(-1, -1);
//     }

//     static constexpr int GLYPH_ROWS = 6;
//     static constexpr int GLYPH_COLS = 16;

//     const int GLYPH_W = this->W / GLYPH_COLS;
//     const int GLYPH_H = this->H / GLYPH_ROWS;

//     int idx = int(c - cmin);
//     int row = idx / GLYPH_COLS;
//     int col = idx % GLYPH_COLS;

//     return ivec2(col*GLYPH_W, row*GLYPH_H);
// }

