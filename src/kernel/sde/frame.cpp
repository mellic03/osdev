#include "frame.hpp"
#include "sde.hpp"
#include <kthread.hpp>
#include <kmalloc.h>
#include <stdio.h>
#include <kernel/memory.hpp>
#include <algorithm>
#include <kernel/log.hpp>

sde::Frame::Frame( ivec2 tl, ivec2 sp )
:   m_global(tl), m_local(tl), m_span(sp),
    m_gx(m_global.x), m_gy(m_global.y),
    m_lx(m_local.x),  m_ly(m_local.y),
    m_w (m_span.x),   m_h (m_span.y),
    m_parent(nullptr)
{

}


sde::Frame::~Frame()
{
    for (Frame *child: m_children)
    {
        delete child;
    }
}


void
sde::Frame::draw()
{
    m_global = m_local;

    if (m_parent)
    {
        m_global += m_parent->m_global;
    }

    if (m_style.border)
    {
        sde::rectOutline(
            m_global, m_span-ivec2(1, 1), m_style.border_color
        );
    }

    for (Frame *F: m_children)
    {
        F->draw();
    }
}




// void
// sde::BorderFrame::draw( sde::Context &ctx )
// {
//     Frame::draw(ctx);
//     ctx.rectOutline(global, span, m_col);
//     // ctx.rect(m_tl+ivec2(24), span-ivec2(48), m_col);
// }


