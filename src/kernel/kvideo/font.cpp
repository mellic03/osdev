#include "font.hpp"


idk::FontBuffer::FontBuffer()
{
    m_data = nullptr;
    name = nullptr;
    W = 0;
    H = 0;
    SZ = 0;
}


vec4 ree_vec4( uint32_t src )
{
    float a = float((src >> 24) & 0xFF) / 255.0f;
    float r = float((src >> 16) & 0xFF) / 255.0f;
    float g = float((src >> 8) & 0xFF)  / 255.0f;
    float b = float((src & 0xFF))       / 255.0f;

    return vec4(r, g, b, a);
}


#include <kmalloc.h>

idk::FontBuffer::FontBuffer( const char *filename, ck_BMP_header *header )
{
    auto &info = header->infoheader;

    uint32_t *data = (uint32_t*)((uint8_t*)header + header->offset);
    m_data = (vec4*)kmalloc(info.width*info.height * sizeof(vec4));

    this->name = filename;
    this->W  = info.width;
    this->H  = info.height;
    this->SZ = W*H;

    for (int i=0; i<W*H; i++)
    {
        m_data[i] = ree_vec4(data[i]);
    }

    for (int i=0; i<H/2; i++)
    {
        for (int j=0; j<W; j++)
        {
            vec4 temp = (*this)[H-1-i][j];
            (*this)[H-1-i][j] = (*this)[i][j];
            (*this)[i][j] = temp;
        }
    }
}



idk::Bounds*
idk::FontBuffer::getGlyph( char c )
{
    static Bounds bounds;

    static constexpr char cmin = ' ';
    static constexpr char cmax = '~';

    if ((cmin<=c && c<=cmax) == false)
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

