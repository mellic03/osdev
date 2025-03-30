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
    m_global = ivec2(0, 0);
    Frame::draw();
    m_global = m_local;
    // sde::hline(0, W, 0, vec4(1.0, 1.0, 1.0, 0.85));
    // sde::hline(0, W, H-1, vec4(1.0, 1.0, 1.0, 0.85));
    // sde::vline(0, 0, H, vec4(1.0, 1.0, 1.0, 0.85));
    // sde::vline(W-1, 0, H-1, vec4(1.0, 1.0, 1.0, 0.85));
}


void sde::WindowContext::clear()
{

}
