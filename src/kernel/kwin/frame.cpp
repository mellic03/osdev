#include "kwin.hpp"


void
kwin::FrameTest::draw( kwin::Context &ctx )
{
    ctx.rectOutline(m_tl, m_sp, m_col);

    ctx.rect(m_tl+ivec2(24), m_sp-ivec2(48), m_col);
}


