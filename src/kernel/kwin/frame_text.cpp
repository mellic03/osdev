#include "frame_text.hpp"
#include "../driver/keyboard.hpp"
#include "../kfs/kfs.hpp"
#include "../log/log.hpp"
#include <kmalloc.h>
#include <stdio.h>


kwin::TextFrame::TextFrame( ivec2 tl, ivec2 sp, idk::FontBuffer *font,
                            const char *text, const Style &style )
:   Frame(tl, sp, vec4(1.0f), style),
    m_font (font),
    m_text (text)
{

}



void
kwin::TextFrame::_reset()
{
    text_dst = ivec2(0, 0);
    text_spn = m_font->getGlyphExtents();
}


void
kwin::TextFrame::_putchar( kwin::Context &ctx, char ch, bool move )
{
    ivec2 &dst   = text_dst;
    ivec2 &span  = text_spn;

    if (move && ch == '\n')
    {
        dst.x = 0;
        dst.y += span.y;
        // _putstr(ctx, ">> ");
        return;
    }

    ivec2 corner = m_font->getGlyphCorner(ch);
    if (corner.x < 0)
    {
        return;
    }

    ctx.blit(
        m_world+dst, corner, span,
        kframebuffer<vec4>(m_font->W, m_font->H, (*m_font)[0])
    );

    if (move == false)
    {
        return;
    }

    dst.x += span.x/2;

    if (dst.x > m_sp.x)
    {
        dst.x = 0;
        dst.y += span.y;
    }
}


void
kwin::TextFrame::_putstr( kwin::Context &ctx, const char *str )
{
    while (*str)
    {
        _putchar(ctx, *(str++));
    }
}


void
kwin::TextFrame::draw( kwin::Context &ctx )
{
    Frame::draw(ctx);

    _reset();
    _putstr(ctx, m_text);

    for (Frame *F: m_children)
    {
        F->m_local.y = text_dst.y;
    }
}





kwin::TerminalFrame::TerminalFrame( ivec2 tl, ivec2 sp, idk::FontBuffer *font, kTTY *tty,
                                    const Style &style )
:   TextFrame (tl, sp, font, nullptr, style),
    m_tty     (tty)
{


}


void
kwin::TerminalFrame::draw( kwin::Context &ctx )
{
    Frame::draw(ctx);

    _reset();
    _putstr(ctx, m_tty->history);
    _putstr(ctx, "[");
    _putstr(ctx, m_tty->getCWD()->name);
    _putstr(ctx, "] ");

    char *pbase = m_tty->prompt;
    char *ptop  = m_tty->ptop;

    for (int i=0; i<ptop-pbase; i++)
    {
        _putchar(ctx, pbase[i]);
    }
    _putchar(ctx, '_', false);

    _putstr(ctx, ptop);

    for (Frame *F: m_children)
    {
        F->m_local.y = text_dst.y;
    }
}


