#pragma once

#include "kwin.hpp"
#include <kdef.h>


namespace kwin
{
    class Context;
    class Frame;
    class FrameTest;
}



class kwin::Frame
{
public:
    ivec2 m_tl, m_sp;
    vec4 m_col;

    Frame( ivec2 tl, ivec2 sp, vec4 c = vec4(1.0f) )
    :   m_tl(tl), m_sp(sp), m_col(c)
    {  }

    virtual ~Frame() = default;
    virtual void draw( kwin::Context& ) = 0;
};




class kwin::FrameTest: public kwin::Frame
{
public:

    using kwin::Frame::Frame;
    virtual void draw( kwin::Context& ) final;

};



