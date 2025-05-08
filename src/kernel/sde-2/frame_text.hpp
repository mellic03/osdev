#pragma once

#include "frame.hpp"
#include "font.hpp"
// #include "../kvideo/font.hpp"
#include "../tty.hpp"

namespace sde
{
    class TextFrame;
    class TerminalFrame;
}


class sde::TextFrame: public sde::Frame
{
protected:
    const char *m_text;
    sde::Font  *m_font;
    ivec2 text_dst, text_spn;

    void _reset();
    void _next_col();
    void _next_row();
    void _putchar( char ch, bool move = true );
    void _putstr( const char* );

public:
    TextFrame( int x, int y, int w, int h, const char *text, sde::Font* );
    virtual void draw() override;

};




class sde::TerminalFrame: public sde::TextFrame
{
private:
    kTTY *m_tty;

public:
    TerminalFrame( int x, int y, int w, int h, kTTY *tty );
    virtual void draw() final;

};
