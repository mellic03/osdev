#include "frame_text.hpp"
#include "sde.hpp"
#include "../driver/keyboard.hpp"
#include <kernel/vfs.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>
#include <stdio.h>


sde::TextFrame::TextFrame( const char *text, idk::FontBuffer *font, ivec2 tl, ivec2 sp )
:   Frame(tl, sp),
    m_text(text),
    m_font(font)
{

}



void
sde::TextFrame::_reset()
{
    text_dst = ivec2(0, 0);
    text_spn = m_font->getGlyphExtents();
}


void
sde::TextFrame::_putchar( char ch, bool move )
{
    ivec2 &dst   = text_dst;
    ivec2 &tspn  = text_spn;

    if (dst.x > m_w || dst.y > m_h)
    {
        return;
    }

    if (move && (ch == '\n'))
    {
        dst.x = 0;
        dst.y += tspn.y;
        return;
    }

    ivec2 corner = m_font->getGlyphCorner(ch);
    if (corner.x < 0)
    {
        return;
    }
    
    sde::blit(
        m_global+dst, corner, tspn,
        kframebuffer<vec4>(m_font->W, m_font->H, (*m_font)[0])
    );

    if (move == false)
    {
        return;
    }

    dst.x += tspn.x;

    if (dst.x > m_w)
    {
        dst.x = 0;
        dst.y += tspn.y;
    }
}


void
sde::TextFrame::_putstr( const char *str )
{
    while (*str)
    {
        _putchar(*(str++));
    }
}


void
sde::TextFrame::draw()
{
    Frame::draw();
    _reset();
    _putstr(m_text);
}




sde::TerminalFrame::TerminalFrame( kTTY *tty, ivec2 tl, ivec2 sp )
:   TextFrame (nullptr, tty->font, tl, sp),
    m_tty     (tty)
{

}


void
sde::TerminalFrame::draw()
{
    Frame::draw();
    m_font = m_tty->font;

    _reset();
    _putstr(m_tty->history);
    _putstr("[");
    _putstr(m_tty->getCWD()->name);
    _putstr("] ");

    char *pbase = m_tty->prompt;
    char *ptop  = m_tty->ptop;

    for (int i=0; i<ptop-pbase; i++)
    {
        _putchar(pbase[i]);
    }
    _putchar('_', false);
    _putstr(ptop);

}
