#include <cringe/font.hpp>
#include <string.h>
#include <kmalloc.h>


cringe::Font::Font()
{
    W = 0;
    H = 0;
    glyph_scale = vec2(1.0f, 1.0f);
    m_img = nullptr;
}


cringe::Font::Font( uint8_t *img, int w, int h )
:   m_img(img),
    W(w), H(h),
    glyph_scale(1.0f)
{

}


ivec2
cringe::Font::getGlyphExtents()
{
    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;
    const int GLYPH_W = this->W / GLYPH_COLS;
    const int GLYPH_H = this->H / GLYPH_ROWS;

    vec2 sp = glyph_scale * vec2(GLYPH_W, GLYPH_H);

    return ivec2(sp);
}


ivec2
cringe::Font::getGlyphCorner( char c )
{
    static constexpr char cmin = ' ';
    static constexpr char cmax = '~';

    if ((cmin<=c && c<=cmax) == false)
    {
        return ivec2(-1, -1);
    }

    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;

    const int GLYPH_W = this->W / GLYPH_COLS;
    const int GLYPH_H = this->H / GLYPH_ROWS;

    int idx = int(c - cmin);
    int row = idx / GLYPH_COLS;
    int col = idx % GLYPH_COLS;

    return ivec2(col*GLYPH_W, row*GLYPH_H);
}

