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
    kframebuffer<vec4> m_buf;
    void _blit( kwin::Context& );

public:
    using kwin::Frame::Frame;

    FrameTTY( ivec2 tl, ivec2 sp, kn_TTY*, idk::FontBuffer* );
    virtual void draw( kwin::Context& ) final;

};


