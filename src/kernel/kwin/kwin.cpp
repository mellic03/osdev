#include "kwin.hpp"
#include <kproc.hpp>
#include <kmalloc.h>
#include <stdio.h>
#include <algorithm>
#include "../log/log.hpp"

#include "frame_tty.hpp"




kwin::Context::Context( int w, int h )
:   m_tl(0, 0),
    m_sp(w, h),
    m_fb(w, h, new vec4[w*h]),
    m_depth(w, h, new uint32_t[w*h])
{
    syslog log("kwin::Context::Context");

    log("fb:    0x%lx", m_fb.buf);
    log("depth: 0x%lx", m_depth.buf);

}


kwin::Context::~Context()
{
    delete m_fb.buf;
    delete m_depth.buf;
}



void kwin::Context::flush()
{
    using namespace idk;

    // kvideo::blit(m_tl, vec2(0, 0), m_sp, m_fb);
    // kvideo::swapBuffers();
    // kmemset<vec4>(m_fb.buf, vec4(0.0f), m_sp.x * m_sp.y);

    // kthread::yield();
}



void
kwin::Context::rectOutline( vec2 tl, vec2 sp, vec4 color )
{
    // uint32_t color = (C.r << 16) + (C.g << 8) + C.b;

    int x0 = std::max(tl.x, 0.0f);
    int x1 = std::min(tl.x+sp.x, m_sp.x-1);
    int y0 = std::max(tl.y, 0.0f);
    int y1 = std::min(tl.y+sp.y, m_sp.y-1);

    for (int x=x0; x<x1; x++)
    {
        m_fb[y0][x] = color;
        m_fb[y0+1][x] = color;
        m_fb[y1][x] = color;
        m_fb[y1-1][x] = color;
    }

    for (int y=y0; y<y1; y++)
    {
        m_fb[y][x0] = color;
        m_fb[y][x0+1] = color;
        m_fb[y][x1] = color;
        m_fb[y][x1-1] = color;
    }
}


void
kwin::Context::rect( vec2 tl, vec2 sp, vec4 color )
{
    // uint32_t color = (C.r << 16) + (C.g << 8) + C.b;

    int x0 = std::max(tl.x, 0.0f);
    int x1 = std::min(tl.x+sp.x, m_sp.x-1);
    int y0 = std::max(tl.y, 0.0f);
    int y1 = std::min(tl.y+sp.y, m_sp.y-1);

    for (int y=y0; y<y1; y++)
    {
        for (int x=x0; x<x1; x++)
        {
            m_fb[y][x] = color;
        }
    }

    kthread::yield();
}


template <typename T>
T idk_mix( const T &x, const T&y, float a )
{
    return (1.0f-a)*x + a*y;
}


vec4 unpack_vec4( uint32_t src )
{
    float a = float((src >> 24) & 0xFF) / 255.0f;
    float r = float((src >> 16) & 0xFF) / 255.0f;
    float g = float((src >> 8) & 0xFF)  / 255.0f;
    float b = float((src & 0xFF))       / 255.0f;

    return vec4(r, g, b, a);
}


void
kwin::Context::blit( vec2 tl0, vec2 tl1, vec2 sp, const kframebuffer<vec4> &buf )
{
    int xmin = std::max(tl0.x, 0.0f);
    int xmax = std::min(tl0.x+sp.x, float(m_fb.w-1));
    int ymin = std::max(tl0.y, 0.0f);
    int ymax = std::min(tl0.y+sp.y, float(m_fb.h-1));

    for (int y=ymin, sy=tl1.y; y<ymax; y++, sy++)
    {
        for (int x=xmin, sx=tl1.x; x<xmax; x++, sx++)
        {
            // int srcx = 2*(sx - tl1.x) + tl1.x;
            // int srcy = 2*(sy - tl1.y) + tl1.y;

            // vec4 src0 = unpack_vec4(buf[srcy+0][srcx+0]);
            // vec4 src1 = unpack_vec4(buf[srcy+0][srcx+1]);
            // vec4 src2 = unpack_vec4(buf[srcy+1][srcx+0]);
            // vec4 src3 = unpack_vec4(buf[srcy+1][srcx+1]);

            // vec4 src = (src0 + src1 + src2 + src3);
            //      src /= 4.0f;

            float a = buf[sy][sx].a;

            if (a > 0.1f)
            {
                m_fb[y][x] = buf[sy][sx];
            }
        }
    }
}