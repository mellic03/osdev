#include "sde.hpp"
#include <kthread.hpp>
#include <kmalloc.h>
#include "kinplace/inplace_vector.hpp"
#include <stdio.h>
#include <algorithm>
#include <kernel/vfs.hpp>
#include <kernel/log.hpp>
#include "frame_tty.hpp"

static sde::WindowContext *m_current = nullptr;
static std::vector<sde::WindowContext*> m_contexts;

void sde_main( void* )
{
    while (true)
    {
        for (auto *ctx: m_contexts)
        {
            sde::makeCurrent(ctx);
            ctx->draw();
            kthread::yield();

            sde::flushContext(ctx);
            kthread::yield();
        }

        kvideo::swapBuffers();
        kthread::yield();
    }
}



sde::WindowContext*
sde::createContext( ivec2 corner, ivec2 span )
{
    m_contexts.push_back(new sde::WindowContext(corner, span));
    return m_contexts.back();
}


void
sde::flushContext( sde::WindowContext *ctx )
{
    auto &src = ctx->rgba;
    auto &tl  = ctx->m_global;
    auto &sp  = ctx->m_span;
    kvideo::blit(tl, ivec2(0, 0), sp, src);
    kmemset<vec4>(src.buf, 0, ctx->W*ctx->H);
}


void
sde::destroyContext( sde::WindowContext *ctx )
{
    int idx = -1;

    for (int i=0; i<m_contexts.size(); i++)
    {
        if (m_contexts[i] == ctx)
        {
            idx = i;
            break;
        }
    }

    if (idx != -1)
    {
        auto *tmp = m_contexts[idx];
        m_contexts[idx] = m_contexts.back();
        m_contexts.back() = tmp;
        m_contexts.pop_back();
        delete ctx;
    }
}


sde::WindowContext*
sde::getCurrent()
{
    return m_current;
}


void
sde::makeCurrent( sde::WindowContext *ctx )
{
    m_current = ctx;
}



bool rect_point_overlap( vec2 tl, vec2 sp, vec2 p )
{
    bool x = p.x == std::clamp(p.x, tl.x, tl.x+sp.x);
    bool y = p.y == std::clamp(p.y, tl.y, tl.y+sp.y);
    return x && y;
}




void
sde::hline( int x0, int x1, int y, const vec4 &color )
{
    auto *ctx = sde::getCurrent();
    auto &dst = ctx->rgba;

    // x0 -= ctx->global.x;
    // x1 -= ctx->global.x;
    // y  -= ctx->global.y;

    if (y < 0 || y >= ctx->H)
    {
        return;
    }

    x0 = std::max(x0, 0);
    x1 = std::min(x1, ctx->W-1);

    for (int x=x0; x<=x1; x++)
    {
        dst[y][x] = color;
    }
}

void
sde::vline( int x, int y0, int y1, const vec4 &color )
{
    auto *ctx = sde::getCurrent();
    auto &dst = ctx->rgba;

    // x  -= ctx->global.x;
    // y0 -= ctx->global.y;
    // y1 -= ctx->global.y;

    if (x < 0 || x >= ctx->W)
    {
        return;
    }

    y0 = std::max(y0, 0);
    y1 = std::min(y1, ctx->H-1);

    for (int y=y0; y<=y1; y++)
    {
        dst[y][x] = color;
    }
}

void
sde::rectOutline( ivec2 tl, ivec2 sp, const vec4 &color )
{
    auto *ctx = sde::getCurrent();
    auto &dst = ctx->rgba;

    int x0 = tl.x;
    int x1 = tl.x + sp.x;
    int y0 = tl.y;
    int y1 = tl.y + sp.y;

    hline(x0, x1, y0, color);
    hline(x0, x1, y1, color);
    vline(x0, y0, y1, color);
    vline(x1, y0, y1, color);
}


void
sde::rect( ivec2 tl, ivec2 sp, const vec4 &color )
{
    auto *ctx = sde::getCurrent();
    auto &dst = ctx->rgba;

    int x0 = std::max(tl.x, 0);
    int x1 = std::min(tl.x+sp.x, ctx->W);
    int y0 = std::max(tl.y, 0);
    int y1 = std::min(tl.y+sp.y, ctx->H);

    for (int y=y0; y<y1; y++)
    {
        for (int x=x0; x<x1; x++)
        {
            dst[y][x] = color;
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



static int blend_mode = 1;

void
sde::blendMode( int mode )
{
    syslog log("sde::blendMode");
    log("blend_mode: %d", mode);
    blend_mode = mode;
}


void
sde::blit( ivec2 tl0, ivec2 tl1, ivec2 sp, const kframebuffer<vec4> &src )
{
    auto *ctx = sde::getCurrent();
    auto &dst = ctx->rgba;

    int xmin = std::max(tl0.x, 0);
    int xmax = std::min(tl0.x+sp.x, ctx->W);
    int ymin = std::max(tl0.y, 0);
    int ymax = std::min(tl0.y+sp.y, ctx->H);

    if (blend_mode == 0)
    {
        for (int y=ymin, sy=tl1.y; y<ymax; y++, sy++)
        {
            for (int x=xmin, sx=tl1.x; x<xmax; x++, sx++)
            {
                dst[y][x] = src[sy][sx];
            }
        }
    }

    else if (blend_mode == 1)
    {
        for (int y=ymin, sy=tl1.y; y<ymax; y++, sy++)
        {
            for (int x=xmin, sx=tl1.x; x<xmax; x++, sx++)
            {
                float a = src[sy][sx].a;
                dst[y][x] = (1.0f - a)*dst[y][x] + a*src[sy][sx];
            }
        }
    }

}

