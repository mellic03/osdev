#include <gui/gui.hpp>
#include <cringe/vec.hpp>
#include <string.h>


ivec2 guiFont::getGlyphExtents()
{
    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;
    const int GLYPH_W = this->w / GLYPH_COLS;
    const int GLYPH_H = this->h / GLYPH_ROWS;

    ivec2 sp = ivec2(GLYPH_W, GLYPH_H);
    // vec2 sp = glyph_scale * vec2(GLYPH_W, GLYPH_H);

    return ivec2(sp);
}


ivec2 guiFont::getGlyphCorner( char c )
{
    static constexpr char cmin = ' ';
    static constexpr char cmax = '~';

    if ((cmin<=c && c<=cmax) == false)
    {
        return ivec2(-1, -1);
    }

    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;

    const int GLYPH_W = this->w / GLYPH_COLS;
    const int GLYPH_H = this->h / GLYPH_ROWS;

    int idx = int(c - cmin);
    int row = idx / GLYPH_COLS;
    int col = idx % GLYPH_COLS;

    return ivec2(col*GLYPH_W, row*GLYPH_H);
}

