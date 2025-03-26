#pragma once
#include "frame.hpp"
#include "../tty.hpp"
#include "../kvideo/font.hpp"

namespace kwin
{
    class FrameTTY;
}



class kwin::FrameTTY: public kwin::Frame
{
private:
    kn_TTY *m_tty;
    idk::FontBuffer *m_font;

public:
    using kwin::Frame::Frame;

    FrameTTY( vec2 tl, vec2 sp, kn_TTY*, idk::FontBuffer* );
    virtual void draw( kwin::Context& ) final;

};


