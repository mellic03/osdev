#include "buffer.hpp"
#include "system/drivers/serial.h"
#include "system/memory/memory.hpp"


idk::video::Buffer::Buffer()
:   m_data(nullptr)
{

}

idk::video::Buffer::Buffer( int w, int h )
:   m_data(new uint32_t[w*h])
{
    W      = w;
    H      = h;
    SZ     = w*h;
}

idk::video::Buffer::Buffer( int w, int h, uint32_t *data )
:   m_data(data)
{
    W      = w;
    H      = h;
    SZ     = w*h;
}


idk::video::Buffer::~Buffer()
{
    if (m_data)
    {
        delete[] m_data;
    }
}



idk::video::FontBuffer::FontBuffer( const char *filename, ck_BMP_header *header )
:   Buffer(
        header->infoheader.width,
        header->infoheader.height,
        (uint32_t*)((uint8_t*)header + header->offset)
    )
{
    serial_printf("[FontBuffer::FontBuffer]\n");
    auto &info = header->infoheader;

    m_data = (uint32_t*)((uint8_t*)header + header->offset);
    serial_printf("[FontBuffer::FontBuffer] m_data=0x%x\n", m_data);

    this->name = filename;
    this->W  = info.width;
    this->H  = info.height;
    this->SZ = W*H;

    // for (int i=0; i<H; i++)
    // {
    //     for (int j=0; j<W; j++)
    //     {
    //         (*this)[H-1-i][j] = data[W*i + j];
    //     }
    // }
}


// idk::video::FontBuffer::~FontBuffer()
// {

// }




idk::Bounds
idk::video::FontBuffer::getGlyph( char c )
{
    static constexpr char min_idx = ' ';
    static constexpr char max_idx = '~';

    if (c < min_idx || c > max_idx)
    {
        return {ivec2(-1), ivec2(-1)};
    }

    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;

    const int GLYPH_W = this->W / GLYPH_COLS;
    const int GLYPH_H = this->H / GLYPH_ROWS;

    int idx = int(c - min_idx);
    int col = idx / GLYPH_COLS;
    int row = idx % GLYPH_COLS;

    ivec2 corner(col*GLYPH_W, row*GLYPH_H);
    ivec2 extents(GLYPH_W, GLYPH_H);

    return {corner, extents};
}

