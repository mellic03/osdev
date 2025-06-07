#include <wm/image.hpp>
#include <wm/render.hpp>
#include <cringe/vec.hpp>
#include <bmp.hpp>
#include <string.h>
#include <kernel/log.hpp>


wm::guiImage::guiImage( void *bmp_addr )
:   guiElement(ivec2(0, 0), ivec2(64, 64))
{
    BMP_File bmp(bmp_addr);
    m_lsp    = ivec2(bmp.w, bmp.h);
    m_gsp    = m_lsp;
    m_pixels = bmp.pixels;
    m_bypp   = bmp.bypp;
    m_pitch  = bmp.pitch;

    // syslog log("guiImage::guiImage");
    // log("w, h:      %d, %d", bmp.w, bmp.h);
    // log("bypp:      %d", bmp.bypp);
    // log("pitch:     %d", bmp.pitch);
    // log("nbytes:    %d", bmp.nbytes);
    // log("");
    // log("m_sp:      %d, %d", m_sp.x, m_sp.y);
    // log("m_bypp:    %d", m_bypp);
    // log("m_pitch:   %d", m_pitch);
    // log("nbytes:    A=%d, B=%d", m_pitch*m_sp.y, m_sp.x*m_sp.y*m_bypp);
}



void wm::guiImage::draw( guiFramebuffer &dst )
{
    if (m_pixels==nullptr || m_gsp==ivec2(0, 0))
        return;
    guiBlitImage(dst, *this, m_gpos);
}



ivec2 wm::guiFont::getGlyphExtents()
{
    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;
    const int GLYPH_W = m_lsp.x / GLYPH_COLS;
    const int GLYPH_H = m_lsp.y / GLYPH_ROWS;

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

    const int GLYPH_W = m_lsp.x / GLYPH_COLS;
    const int GLYPH_H = m_lsp.y / GLYPH_ROWS;

    int idx = int(c - cmin);
    int row = idx / GLYPH_COLS;
    int col = idx % GLYPH_COLS;

    return ivec2(col*GLYPH_W, row*GLYPH_H);
}

