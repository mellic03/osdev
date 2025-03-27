#include "frame_tty.hpp"
#include "../driver/keyboard.hpp"
#include "../kfs/kfs.hpp"
#include <kmalloc.h>
#include <stdio.h>


kwin::FrameTTY::FrameTTY( ivec2 tl, ivec2 sp, kn_TTY *tty, idk::FontBuffer *font )
:   Frame  (tl, sp, vec4(1.0f)),
    m_tty  (tty),
    m_font (font)
{
    m_buf = kframebuffer<vec4>(sp.x, sp.y, (vec4*)kmalloc(sp.x * sp.y * sizeof(vec4)));
}



void
kwin::FrameTTY::_blit( kwin::Context &ctx )
{
    kvideo::blit(m_tl, vec2(0.0f), m_sp, ctx.m_fb, m_buf);

    {
        int row = (m_tty->CSR) / m_tty->W;
        int col = (m_tty->CSR) % m_tty->W;

        auto *bd = m_font->getGlyph('_');

        vec2 src  = bd->corner;
        vec2 span = bd->extents;
        
        vec2 dst  = vec2(
            m_tl.x + (span.x/2)*col,
            m_tl.y + span.y*row
        );
        
        kvideo::blit(dst, src, span, ctx.m_fb, kframebuffer<vec4>(m_font->W, m_font->H, (*m_font)[0]));
    }
    ctx.rectOutline(m_tl, m_sp, m_col);
}



void
kwin::FrameTTY::draw( kwin::Context &ctx )
{
    auto &tty  = *m_tty;
    auto &font = *m_font;
    char *data = tty.data;

    if (tty.dirty == false)
    {
        _blit(ctx);
        return;
    }

    kmemset<vec4>(m_buf.buf, vec4(0.0f), m_sp.x * m_sp.y);
    tty.dirty = false;

    for (int csr=0; csr<tty.W*tty.H; csr++)
    {
        int row = csr / tty.W;
        int col = csr % tty.W;

        char ch = tty.data[csr];

        if (ch == '\n' || ch == '\0')
        {
            col = 0;
            row += 1;
            csr = tty.W*row + col - 1;
            continue;
        }

        auto *bd = font.getGlyph(ch);

        if (!bd)
        {
            continue;
        }

        vec2 src  = bd->corner;
        vec2 span = bd->extents;

        vec2 dst  = vec2(
            (span.x/2)*col,
            span.y*row
        );

        kvideo::blit(dst, src, span, m_buf, kframebuffer<vec4>(font.W, font.H, font[0]));
    }

    _blit(ctx);
}


// {
//     // auto *font = m_font;
//     // const char *data = m_tty->data();
//     // int csr = 0;

//     // while (csr < m_tty->CSR)
//     // {
//     //     int  row = csr / m_tty->W;
//     //     int  col = csr % m_tty->W;
//     //     char val = data[csr];
//     //     csr += 1; 

//     //     auto *bd = font->getGlyph(val);

//     //     if (!bd)
//     //     {
//     //         continue;
//     //     }

//     //     ivec2 src  = bd->corner;
//     //     ivec2 span = bd->extents;

//     //     ivec2 dst  = ivec2(
//     //         m_tl.x + span.x*col,
//     //         m_tl.y + span.y*row
//     //     );

//     //     video.blit(dst, src, span, buf_wrapper<uint32_t>(font->W, (*font)[0]), m_depth);
//     // }
// }