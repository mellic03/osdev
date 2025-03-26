#include "kwin.hpp"


void
kwin::FrameTest::draw( kwin::Context &ctx )
{
    ctx.rectOutline(m_tl, m_sp, m_col);

    ctx.rect(m_tl+vec2(24), m_sp-vec2(48), m_col);
}


