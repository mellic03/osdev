#include "font.hpp"
#include "../gx/gx.hpp"
#include "../kvideo/texture.hpp"
#include <string.h>
#include <kmalloc.h>
#include <kernel/vfs.hpp>


sde::Font::Font()
{
    W = 0;
    H = 0;
    glyph_scale = vec2(1.0f, 1.0f);
    m_tex = 0;
}


sde::Font::Font( uint32_t tex, int w, int h )
:   m_tex(tex),
    W(w), H(h),
    glyph_scale(1.0f)
{

}

sde::Font::Font( const char *filepath )
{
    auto *fh = vfsFindFile(filepath);
    BMP_File bmp(fh->addr);

    m_tex = sde::TextureLoad<float>(bmp);
    W     = bmp.w;
    H     = bmp.h;
    glyph_scale = vec2(1.0f);

    // m_tex = gxCreateTexture(GX_RGBA8, bmp.data, W, H);
    // W = bmp.w;
    // H = bmp.h;
    // glyph_scale = vec2(1.0f, 1.0f);

    // kfree(tex.pixels);
}



ivec2
sde::Font::getGlyphExtents()
{
    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;
    const int GLYPH_W = this->W / GLYPH_COLS;
    const int GLYPH_H = this->H / GLYPH_ROWS;

    vec2 sp = glyph_scale * vec2(GLYPH_W, GLYPH_H);

    return ivec2(sp);
}


ivec2
sde::Font::getGlyphCorner( char c )
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

