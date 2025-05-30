#include <wm/image.hpp>
#include <wm/render.hpp>
#include <cringe/vec.hpp>
#include <bmp.hpp>
#include <string.h>



wm::guiImage::guiImage( void *bmp_addr )
:   guiElement(ivec2(0, 0), ivec2(50, 50))
{
    BMP_File bmp(bmp_addr);
    m_sp     = ivec2(bmp.w, bmp.h);
    m_pixels = bmp.pixels;
    m_bpp    = bmp.bpp;

}



void wm::guiImage::draw( guiFramebuffer &dst )
{
    if (m_pixels==nullptr || m_sp==ivec2(0, 0))
        return;
    guiBlitImage(dst, *this, m_tl);
}



ivec2 wm::guiFont::getGlyphExtents()
{
    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;
    const int GLYPH_W = m_sp.x / GLYPH_COLS;
    const int GLYPH_H = m_sp.y / GLYPH_ROWS;

    ivec2 sp = ivec2(GLYPH_W, GLYPH_H);
    // vec2 sp = glyph_scale * vec2(GLYPH_W, GLYPH_H);

    return ivec2(sp);
}


ivec2 wm::guiFont::getGlyphCorner( char c )
{
    static constexpr char cmin = ' ';
    static constexpr char cmax = '~';

    if ((cmin<=c && c<=cmax) == false)
    {
        return ivec2(-1, -1);
    }

    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;

    const int GLYPH_W = m_sp.x / GLYPH_COLS;
    const int GLYPH_H = m_sp.y / GLYPH_ROWS;

    int idx = int(c - cmin);
    int row = idx / GLYPH_COLS;
    int col = idx % GLYPH_COLS;

    return ivec2(col*GLYPH_W, row*GLYPH_H);
}

