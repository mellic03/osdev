#ifndef __is_kernel
    #define __is_kernel
#endif

#include "gx.hpp"
#include "command.hpp"

#include "../kvideo/kvideo.hpp"
#include "../boot/boot.hpp"
#include "../cpu/cpu.hpp"
#include <kernel/clock.hpp>
#include <kernel/log.hpp>
#include <kernel/vfs.hpp>
#include <kmalloc.h>
#include <kthread.hpp>
#include <kmemxx.hpp>
#include <string.h>
#include <algorithm>
#include <atomic>
#include <type_traits>



// std::atomic_int gx_flushbarrier;
// static std::atomic_int gx_entryCount;

static gxDrawCmd  gx_cmdBuf[256];
static uint8_t    gx_cmdHead = 0;
static uint8_t    gx_cmdTail = 0;
static int        gx_cmdSize = 0;
static std::mutex gx_mutex;
static uint32_t   gx_renderColor;

#define MUTEX_LOCK(Content)\
    gx_mutex.lock();\
    Content\
    gx_mutex.unlock();\




// __attribute__((constructor))
// static void gx_init()
// {

//     gx_entryCount.store(0);
// }


void gx_main()
{
    // int entryid = gx_entryCount++;
    // syslog::kprintf("[gx_main] entryid=%d\n", entryid);

    // if (entryid == 0)
    // {
        // syslog::kprintf("[gx_main] Yeet\n");
        gxEnable(GX_BLEND);
        gxEnable(GX_DEPTH_TEST);
        gxDisable(GX_STENCIL_TEST);
    
        gxViewport(0, 0, kvideo::W, kvideo::H);
        gxClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    
        gx_renderColor = gxCreateTextureExplicit(
            gxResourceFlag_ExplicitAlloc | gxResourceFlag_ExplicitFree,
            GX_UNSIGNED_BYTE+kvideo::stride, kvideo::backbuffer, kvideo::W, kvideo::H
        );
    
        auto *ctx = gxGetCurrent();
        gx_mutex.lock();
        
        ctx->target.color   = gx_renderColor;
        ctx->target.depth   = gxCreateTexture(GX_R32F, nullptr, kvideo::W, kvideo::H);
        ctx->target.stencil = gxCreateTexture(GX_R8,   nullptr, kvideo::W, kvideo::H);
        
        gx_mutex.unlock();
    // }

    // else
    // {
    //     syslog::kprintf("[gx_main] Ree %d\n", entryid);
    // }

    gxDrawCmd cmd{};
    bool has_cmd = false;

    while (true)
    {
        has_cmd = false;

        MUTEX_LOCK(
            if (gx_cmdSize > 0)
            {
                has_cmd = true;
                cmd = gx_cmdBuf[gx_cmdHead++];
                gx_cmdSize -= 1;
            }
        )

        if (has_cmd)
        {
            gx_ExecCommand(cmd);
        }
    }

    while (true) { asm volatile ("cli; hlt"); }
}


// void gx_waitBarrier( std::atomic_int &barrier )
// {
//     barrier--;
//     while (barrier.load() > 0)
//     {
//         kthread::yield();
//     }
// }



void gxEnable( gxEnum e )
{
    auto *ctx = gxGetCurrent();

    switch (e)
    {
        default: break;
        case GX_BLEND:        ctx->conf.blend   = true; break;
        case GX_DEPTH_TEST:   ctx->conf.depth   = true; break;
        case GX_STENCIL_TEST: ctx->conf.stencil = true; break;
    }
}

void gxDisable( gxEnum e )
{
    auto *ctx = gxGetCurrent();

    switch (e)
    {
        default: break;
        case GX_BLEND:        ctx->conf.blend   = false; break;
        case GX_DEPTH_TEST:   ctx->conf.depth   = false; break;
        case GX_STENCIL_TEST: ctx->conf.stencil = false; break;
    }
}

void gxViewport( int x, int y, int w, int h )
{
    auto *ctx = gxGetCurrent();
    MUTEX_LOCK( ctx->viewport = ivec4(x, y, w, h); )
}


template <typename T>
void gxPushCommand( gxDrawCmd_ type, const T &cmd )
{
    auto *ctx = gxGetCurrent();
    {
        gx_mutex.lock();

        auto &dst = gx_cmdBuf[gx_cmdTail];
        gx_cmdTail = (gx_cmdTail+1) % 255;
        gx_cmdSize += 1;

        dst.type = type;
        dst.conf = ctx->conf;
        auto &data = dst.data;

        if constexpr (std::is_same_v<T, gxDrawCmdBlit>)  data.blit  = cmd;
        if constexpr (std::is_same_v<T, gxDrawCmdLine>)  data.line  = cmd;
        if constexpr (std::is_same_v<T, gxDrawCmdRect>)  data.rect  = cmd;
        if constexpr (std::is_same_v<T, gxDrawCmdTris>)  data.tris  = cmd;

        gx_mutex.unlock();
    }
}


void gxFlush()
{
    // const gxTrianglePrimitive P = {
    //     {vec3(10, 10, 0), vec3(200, 50, 0), vec3(100, 200, 0)},
    //     {0, 0, 0},
    //     {0, 0, 0},
    //     {vec3(0), vec3(0), vec3(0)},
    //     {vec2(10.0f/500.0f, 10.0f/500.0f), vec2(200.0f/500.0f, 50.0f/500.0f), vec2(100.0f/500.0f, 500.0f/500.0f)}
    // };
    // gx_rasterize(P)

    int count = 255;
    while (count > 0)
    {
        MUTEX_LOCK(count = gx_cmdSize;)
        kthread::yield();
    }

    kvideo::swapBuffers();
}


float gxDepthSample( int x, int y )
{
    auto  *ctx  = gxGetCurrent();
    auto  *tex  = gxGetTexture(ctx->target.depth);
    float *data = (float*)(tex->data);

    // int x = int(tex->w*u + 0.5f) % tex->w;
    // int y = int(tex->h*v + 0.5f) % tex->h;

    return data[tex->w*y + x];
}



void gxTintColor( float r, float g, float b, float a )
{
    gxGetCurrent()->conf.tint = vec4(r, g, b, a);
}

void gxClearColor( float r, float g, float b, float a )
{
    gxGetCurrent()->clearcolor = vec4(r, g, b, a);
}

void gxClearDepth( float d )
{
    gxGetCurrent()->cleardepth = d;
}

void gxClearStencil( int s )
{
    gxGetCurrent()->clearstencil = s;
}

void gxClear( uint32_t mask )
{
    gxPushContext();
    gxDisable(GX_BLEND);
    gxDisable(GX_DEPTH_TEST);
    gxDisable(GX_STENCIL_TEST);

    auto *ctx = gxGetCurrent();

    if (mask & GX_COLOR_BUFFER_BIT)
    {
        gxClearTexture(ctx->target.color);
    }

    if (mask & GX_DEPTH_BUFFER_BIT)
    {
        uint32_t texid = ctx->target.depth;
        gxTexture *tex = gxGetTexture(texid);
        gxDrawRect(texid, {0, 0, tex->w, tex->h}, true, 0, vec4(ctx->cleardepth));
    }

    if (mask & GX_STENCIL_BUFFER_BIT)
    {
        uint32_t texid = ctx->target.stencil;
        gxTexture *tex = gxGetTexture(texid);
        gxDrawRect(texid, {0, 0, tex->w, tex->h}, true, 0, vec4(ctx->clearstencil));
    }

    gxPopContext();
}


void gxClearTexture( uint32_t texid )
{
    gxPushContext();
    gxDisable(GX_BLEND);
    gxDisable(GX_DEPTH_TEST);

    auto *ctx = gxGetCurrent();
    auto *tex = gxGetTexture(texid);
    gxDrawRect(texid, {0, 0, tex->w, tex->h}, true, 0, ctx->clearcolor);

    gxPopContext();
}



void gxBlitTexture( uint32_t dstid, uint32_t srcid, const gxRect &dstrect, const gxRect &srcrect )
{
    gxDrawCmdBlit cmd = {
        .dst = dstid,
        .src = srcid,
        .dstrect = dstrect,
        .srcrect = srcrect
    };

    gxPushCommand(gxDrawCmd_Blit, cmd);
}

void gxBlitTexture( uint32_t src, const gxRect &dstrect )
{
    auto *stex = gxGetTexture(src);
    gxBlitTexture(gx_renderColor, src, dstrect, {0, 0, stex->w, stex->h});
}

void gxBlitTexture( uint32_t src, int x, int y )
{
    auto *stex = gxGetTexture(src);
    gxBlitTexture(src, {x, y, stex->w, stex->h});
}

void gxBlitTexture( uint32_t src, const gxRect &dstrect, const gxRect &srcrect )
{
    gxBlitTexture(gx_renderColor, src, dstrect, srcrect);
}

void gxBlitTexture( uint32_t dst, uint32_t src, const gxRect &dstrect )
{
    auto *stex = gxGetTexture(src);
    gxBlitTexture(dst, src, dstrect, {0, 0, stex->w, stex->h});
}





void gxDrawRect( uint32_t dstid, const gxRect &rect, bool fill, int border,
                 const vec4 &fillColor, const vec4 &borderColor )
{
    gxDrawCmdRect cmd = {
        .dst    = dstid,
        .rect   = rect,
        .fill   = fill,
        .border = border,
        .fillColor = fillColor,
        .borderColor = borderColor
    };

    gxPushCommand(gxDrawCmd_Rect, cmd);
}

void gxDrawRect( const gxRect &rect, bool fill, int border,
                 const vec4 &fillColor, const vec4 &borderColor )
{
    gxDrawRect(gx_renderColor, rect, fill, border, fillColor, borderColor);
}

