#include "context.hpp"
#include "frame.hpp"
#include "sde.hpp"
#include <kthread.hpp>
#include <stdio.h>
#include <algorithm>
#include <kernel/vfs.hpp>
#include <kernel/log.hpp>



sde::WindowContext::WindowContext( ivec2 tl, ivec2 sp )
:   Frame(tl, sp),
    W(sp.x), H(sp.y),
    rgba  (W, H, new vec4[W*H]),
    depth (W, H, new uint32_t[W*H])
{

}


sde::WindowContext::~WindowContext()
{
    delete[] rgba.buf;
    delete[] depth.buf;
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
