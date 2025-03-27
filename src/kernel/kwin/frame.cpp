#include "kwin.hpp"
#include <kproc.hpp>
#include <kmalloc.h>
#include <stdio.h>
#include <kernel/memory.hpp>
#include <algorithm>
#include "../log/log.hpp"

kwin::Frame::Frame( ivec2 tl, ivec2 sp, vec4 c, const Style &style )
:   m_local(tl), m_world(tl), m_tl(m_world), m_sp(sp), m_col(c), m_style(style)
{

}



void
kwin::Frame::updateTransforms()
{
    m_world = m_local;

    if (m_parent)
    {
        m_world += m_parent->m_world;
    }

    for (Frame *F: m_children)
    {
        F->updateTransforms();
    }
}


void
kwin::Frame::draw( kwin::Context &ctx )
{
    if (m_style.border.a > 0.0f)
    {
        ctx.rectOutline(m_world, m_sp, m_style.border);
    }

    for (Frame *F: m_children)
    {
        F->draw(ctx);
    }
}




// void
// kwin::BorderFrame::draw( kwin::Context &ctx )
// {
//     Frame::draw(ctx);
//     ctx.rectOutline(m_world, m_sp, m_col);
//     // ctx.rect(m_tl+ivec2(24), m_sp-ivec2(48), m_col);
// }


