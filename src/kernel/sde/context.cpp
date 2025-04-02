#ifndef __is_kernel
    #define __is_kernel
#endif

#include "context.hpp"
#include "frame.hpp"
#include "sde.hpp"
#include <kthread.hpp>
#include <stdio.h>
#include <algorithm>
#include <kernel/vfs.hpp>
#include <kernel/log.hpp>
// #include <kmalloc.h>



sde::WindowContext::WindowContext( ivec2 tl, ivec2 sp )
:   Frame(tl, sp),
    ID(ctx_id++),
    W(sp.x), H(sp.y),
    // rgba  (W, H, (vec4*)kmallocAligned(W*H*sizeof(vec4), 16)),
    rgba  (W, H, (vec4*)kmalloc(W*H*sizeof(vec4))),
    depth (W, H, (uint32_t*)kmalloc(W*H*sizeof(vec4)))
{

}


sde::WindowContext::~WindowContext()
{
    kfree(rgba.buf);
    kfree(depth.buf);
}


void sde::WindowContext::draw()
{
    m_global = m_local;

    if (m_parent)       m_global += m_parent->m_global;
    if (m_style.fill)   sde::rect(m_local, m_span-1, m_style.fill_color);
    if (m_style.border) sde::rectOutline(m_local, m_span-1, m_style.border_color);

    Frame::draw();
    // sde::hline(0, W, 0, vec4(1.0, 1.0, 1.0, 0.85));
    // sde::hline(0, W, H-1, vec4(1.0, 1.0, 1.0, 0.85));
    // sde::vline(0, 0, H, vec4(1.0, 1.0, 1.0, 0.85));
    // sde::vline(W-1, 0, H-1, vec4(1.0, 1.0, 1.0, 0.85));
}


void sde::WindowContext::clear()
{

}
