#pragma once
#include "frame.hpp"

namespace kwin
{
    class FrameTTY;
}



class kwin::FrameTTY: public kwin::Frame
{
private:
    int m_row, m_col;
    uint8_t m_buf[25*80];

public:
    using kwin::Frame::Frame;

    FrameTTY( ivec2 tl, ivec2 sp );
    virtual void draw( kwin::Context& ) final;

};


