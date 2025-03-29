#pragma once

#include "frame.hpp"
#include "../kvideo/font.hpp"
#include "../tty.hpp"

namespace sde
{
    class TextFrame;
    class TerminalFrame;
}


class sde::TextFrame: public sde::Frame
{
protected:
    const char      *m_text;
    idk::FontBuffer *m_font;
    ivec2 text_dst, text_spn;

    void _reset();
    void _putchar( char ch, bool move = true );
    void _putstr( const char* );

public:
    TextFrame( const char *text, idk::FontBuffer*, ivec2 tl, ivec2 sp );
    virtual void draw() override;

};




class sde::TerminalFrame: public sde::TextFrame
{
private:
    kTTY *m_tty;

public:
    TerminalFrame( kTTY *tty, ivec2 tl=ivec2(0), ivec2 sp=ivec2(100) );
    virtual void draw() final;

};
