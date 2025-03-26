#include "kwin.hpp"
#include <kproc.hpp>
#include <kmalloc.h>
#include <stdio.h>
#include <algorithm>
#include "../log/log.hpp"

#include "frame_tty.hpp"




kwin::Context::Context( int w, int h )
:   m_tl(0, 0),
    m_sp(w, h)
    // frames (32, nullptr)
{
    syslog log("kwin::Context::Context");

    fb    = kframebuffer<uint32_t>(w, h, (uint32_t*)kmalloc(w*h * sizeof(uint32_t)));
    depth = kframebuffer<uint32_t>(w, h, (uint32_t*)kmalloc(w*h * sizeof(uint32_t)));

    log("fb:    0x%lx", fb.buf);
    log("depth: 0x%lx", depth.buf);

}



void kwin::Context::flush()
{
    using namespace idk;
    
    uint32_t *dst = kvideo::backbuffer.buf;
    uint32_t *src = fb.buf;

    kvideo::blit(m_tl, ivec2(0, 0), m_sp, fb);

    kvideo::swapBuffers();
    kproc_yield();
}



void kwin::Context::rectOutline( ivec2 tl, ivec2 sp, uvec4 C )
{
    uint32_t color = (C.r << 16) + (C.g << 8) + C.b;

    int x0 = std::max(tl.x, 0);
    int x1 = std::min(tl.x+sp.x, m_sp.x-1);
    int y0 = std::max(tl.y, 0);
    int y1 = std::min(tl.y+sp.y, m_sp.y-1);

    for (int x=x0; x<x1; x++)
    {
        fb[y0][x] = color;
        fb[y1][x] = color;
    }

    for (int y=y0; y<y1; y++)
    {
        fb[y][x0] = color;
        fb[y][x1] = color;
    }
}


void kwin::Context::rect( ivec2 tl, ivec2 sp, uvec4 C )
{
    uint32_t color = (C.r << 16) + (C.g << 8) + C.b;

    int x0 = std::max(tl.x, 0);
    int x1 = std::min(tl.x+sp.x, m_sp.x-1);
    int y0 = std::max(tl.y, 0);
    int y1 = std::min(tl.y+sp.y, m_sp.y-1);

    for (int y=y0; y<y1; y++)
    {
        for (int x=x0; x<x1; x++)
        {
            this->fb[y][x] = color;
        }
    }

    kproc_yield();
}

