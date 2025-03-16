#include "font.hpp"
#include "system/drivers/serial.h"
#include "system/memory/memory.hpp"


// idk::video::Buffer::Buffer()
// :   m_data(nullptr)
// {

// }

// idk::video::Buffer::Buffer( int w, int h )
// :   m_data(new uint32_t[w*h])
// {
//     W      = w;
//     H      = h;
//     SZ     = w*h;
// }

// idk::video::Buffer::Buffer( int w, int h, uint32_t *data )
// :   m_data(data)
// {
//     W      = w;
//     H      = h;
//     SZ     = w*h;
// }


// idk::video::Buffer::~Buffer()
// {
//     if (m_data)
//     {
//         delete[] m_data;
//     }
// }




idk::FontBuffer::FontBuffer( const char *filename, ck_BMP_header *header )
{
    auto &info = header->infoheader;
    m_data = (uint32_t*)((uint8_t*)header + header->offset);

    this->name = filename;
    this->W  = info.width;
    this->H  = info.height;
    this->SZ = W*H;

    for (int i=0; i<H/2; i++)
    {
        for (int j=0; j<W; j++)
        {
            uint32_t temp = (*this)[H-1-i][j];
            (*this)[H-1-i][j] = (*this)[i][j];
            (*this)[i][j] = temp;
        }
    }
}



// bool
// idk::FontBuffer::hasGlyph( char c )
// {
//     static constexpr char cmin = ' ';
//     static constexpr char cmax = '~';
//     return (cmin <= c) && (c <= cmax);
// }


idk::Bounds*
idk::FontBuffer::getGlyph( char c )
{
    static Bounds bounds;

    static constexpr char cmin = ' ';
    static constexpr char cmax = '~';

    if ((c < cmin) && (cmax < c))
    {
        return nullptr;
    }

    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;

    const int GLYPH_W = this->W / GLYPH_COLS;
    const int GLYPH_H = this->H / GLYPH_ROWS;

    int idx = int(c - cmin);
    int row = idx / GLYPH_COLS;
    int col = idx % GLYPH_COLS;

    ivec2 corner(col*GLYPH_W, row*GLYPH_H);
    ivec2 extents(GLYPH_W, GLYPH_H);

    bounds = {corner, extents};
    return &bounds;
}

