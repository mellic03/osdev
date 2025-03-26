#include "frame_tty.hpp"
#include "../driver/keyboard.hpp"
#include "../kfs/kfs.hpp"
#include <stdio.h>


kwin::FrameTTY::FrameTTY( vec2 tl, vec2 sp, kn_TTY *tty, idk::FontBuffer *font )
:   Frame  (tl, sp, vec4(1.0f)),
    m_tty  (tty),
    m_font (font)
{

}




void
kwin::FrameTTY::draw( kwin::Context &ctx )
{
    auto &tty  = *m_tty;
    auto &font = *m_font;
    char *data = tty.data;

    if (tty.dirty)
    {
        kmemset<vec4>(ctx.m_fb.buf, vec4(0, 0, 0, 255), ctx.m_sp.x * ctx.m_sp.y);
        tty.dirty = false;
    }


    {
        int row = (tty.CSR) / tty.W;
        int col = (tty.CSR) % tty.W;

        auto *bd = font.getGlyph('_');

        vec2 src  = bd->corner;
        vec2 span = bd->extents;
        
        vec2 dst  = vec2(
            m_tl.x + span.x*col,
            m_tl.y + span.y*row
        );
        
        ctx.blit(dst, src, span, kframebuffer<vec4>(font.W, font.H, font[0]));
    }



    for (int csr=0; csr<tty.CSR; csr++)
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
            m_tl.x + span.x*col,
            m_tl.y + span.y*row
        );

        ctx.blit(dst, src, span, kframebuffer<vec4>(font.W, font.H, font[0]));
    }


    ctx.rectOutline(m_tl, m_sp, m_col);
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