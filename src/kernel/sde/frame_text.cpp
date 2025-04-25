#include "frame_text.hpp"
#include "sde.hpp"
#include "../driver/keyboard.hpp"
#include <kernel/vfs.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>
#include <stdio.h>


sde::TextFrame::TextFrame( int x, int y, int w, int h, const char *text, sde::Font *font )
:   Frame(x, y, w, h),
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
sde::TextFrame::_next_col()
{
    text_spn = m_font->getGlyphExtents();
    text_dst.x += text_spn.x;
    if (text_dst.x > m_span.x)
        _next_row();
}

void
sde::TextFrame::_next_row()
{
    text_spn = m_font->getGlyphExtents();
    text_dst.x  = 0;
    text_dst.y += text_spn.y;
}


void
sde::TextFrame::_putchar( char ch, bool move )
{
    int    col   = 0;
    ivec2 &dst   = text_dst;
    ivec2 &tspn  = text_spn;

    if (dst.y > m_span.y)
    {
        return;
    }

    if (dst.x > m_span.x)
    {
        _next_row();
    }

    if (move && (ch == '\n'))
    {
        col   = 0;
        dst.x = 0;
        dst.y += tspn.y;
        return;
    }

    ivec2 corner = m_font->getGlyphCorner(ch);
    if (corner.x < 0)
    {
        return;
    }

    gxBlitTexture(
        m_font->m_tex,
        {m_wcorner.x+dst.x, m_wcorner.y+dst.y, tspn.x, tspn.y, m_depth},
        {corner.x, corner.y, tspn.x, tspn.y}
    );

    if (move == false)
    {
        return;
    }

    col   += 1;
    dst.x += tspn.x;

    if (dst.x > m_span.x)
    {
        col   = 0;
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




sde::TerminalFrame::TerminalFrame( int x, int y, int w, int h, kTTY *tty )
:   TextFrame (x, y, w, h, nullptr, tty->font),
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
    _putstr(m_tty->prompt);
}
