#pragma once

#include "frame.hpp"
#include "../kvideo/font.hpp"
#include "../tty.hpp"

namespace kwin
{
    class TextFrame;
    class TerminalFrame;
}



class kwin::TextFrame: public kwin::Frame
{
protected:
    idk::FontBuffer *m_font;
    const char *m_text;
    ivec2 text_dst;
    ivec2 text_spn;
    void _reset();
    void _putchar( kwin::Context&, char ch, bool move = true );
    void _putstr( kwin::Context&, const char* );

public:
    TextFrame( ivec2 tl, ivec2 sp, idk::FontBuffer*, const char *text, const Style &s=Style() );
    virtual void draw( kwin::Context& ) override;

};




class kwin::TerminalFrame: public kwin::TextFrame
{
private:
    kTTY *m_tty;

public:
    TerminalFrame( ivec2 tl, ivec2 sp, kTTY *tty,
                   const Style &s=Style() );

    virtual void draw( kwin::Context& ) final;

};


