#ifndef __is_kernel
    #define __is_kernel
#endif

#include "kvideo.hpp"
#include "../boot/boot.hpp"
#include <kernel/log.hpp>
#include <kmalloc.h>
#include <kthread.hpp>
#include <kmemxx.h>
#include <string.h>
#include <algorithm>


int kvideo::W;
int kvideo::H;
uint32_t *kvideo::frontbuffer;
uint32_t *kvideo::backbuffer;


void kvideo::init( uintptr_t fbres )
{
    auto *res = (limine_framebuffer_response*)fbres;

    syslog log("kvideo::init");
    auto *fb = res->framebuffers[0];

    kvideo::W = fb->width;
    kvideo::H = fb->height;

    kvideo::frontbuffer = (uint32_t*)fb->address;
    kvideo::backbuffer  = (uint32_t*)kmalloc(W*H*sizeof(uint32_t));

    log("framebuffer 0: %dx%dx%d  0x%lx", W, H, fb->bpp, fb->address);

}


void kvideo::swapBuffers()
{
    kmemcpy<uint32_t>(frontbuffer, backbuffer, W*H*sizeof(uint32_t));
    kmemset<uint32_t>(backbuffer, 0, W*H*sizeof(uint32_t));
}



void kvideo::fill( uint32_t C )
{
    __uint128_t color = (__uint128_t(C)<<96) + (__uint128_t(C)<<64)
                      + (__uint128_t(C)<<32) +  __uint128_t(C);

    kmemset<uint128_t>(backbuffer, color, W*H*sizeof(uint32_t));
}

void kvideo::fill( const vec4 &rgba )
{
    uint32_t r = uint32_t(255.0f * rgba.r);
    uint32_t g = uint32_t(255.0f * rgba.g);
    uint32_t b = uint32_t(255.0f * rgba.b);

    fill(255 + (r<<16) + (g<<8) + (b<<0));
}

void kvideo::fill( const vec3 &rgb )
{
    fill(vec4(rgb, 1.0));
}


void kvideo::blit( ivec2 dst, ivec2 src, ivec2 sp,
                   kframebuffer<vec4> &dstbuf,
                   const kframebuffer<vec4> &srcbuf )
{
    int xmin = std::max(dst.x, 0);
    int xmax = std::min(dst.x+sp.x, dstbuf.w-1);
    int ymin = std::max(dst.y, 0);
    int ymax = std::min(dst.y+sp.y, dstbuf.h-1);

    for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
    {
        for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
        {
            dstbuf[y][x] = srcbuf[sy][sx];
        }
    }


    // int dx = std::min(xmax-xmin, sp.x);

    // for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
    // {
    //     kmemcpy<uint128_t>(dstbuf[y], srcbuf[sy], dx*sizeof(vec4));
    // }
}


struct kvideo_blit_cmd
{
    ivec2 dst, src, span;
    const vec4 *buf;
    int bufw, bufh;
};

// static kvideo_blit_cmd blit_buffer[32];
// static std::mutex      blit_mutex;
// static int blit_idx = 0;

void kvideo_blit( ivec2 dst, ivec2 src, ivec2 sp, const vec4 *buf, int bufw, int )
{
    using namespace kvideo;

    int xmin = std::max(dst.x, 0);
    int xmax = std::min(dst.x+sp.x, W-1);
    int ymin = std::max(dst.y, 0);
    int ymax = std::min(dst.y+sp.y, H-1);

    for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
    {
        for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
        {
            vec4 rgba = buf[bufw*sy + sx];

            uint32_t r = uint32_t(255.0f * rgba.r);
            uint32_t g = uint32_t(255.0f * rgba.g);
            uint32_t b = uint32_t(255.0f * rgba.b);

            backbuffer[W*y + x] = (255<<24) + (r<<16) + (g<<8) + (b<<0);
        }
    }
}


void kvideo_blit_main( void* )
{
    // asm volatile ("cli");

    // while (true)
    // {
    //     // blit_mutex.lock();

    //     if (0 <= blit_idx && blit_idx <= 31)
    //     {
    //         auto cmd = blit_buffer[blit_idx--];
    //         kvideo_blit(cmd.dst, cmd.src, cmd.span, cmd.buf, cmd.bufw, cmd.bufh);
    //     }

    //     kthread::yield();
    //     // blit_mutex.unlock();
    // }
    // while (true) { asm volatile ("cli; hlt"); }
}

void kvideo::blit( ivec2 dst, ivec2 src, ivec2 sp,
                   const kframebuffer<vec4> &buf )
{
    // {
    //     // blit_mutex.lock();
    //     blit_idx = std::max(0, std::min(blit_idx, 30));
    //     blit_buffer[blit_idx++] = {dst, src, sp, buf.buf, buf.w, buf.h};
    //     // blit_mutex.unlock();
    // }

    using namespace kvideo;

    int xmin = std::max(dst.x, 0);
    int xmax = std::min(dst.x+sp.x, W-1);
    int ymin = std::max(dst.y, 0);
    int ymax = std::min(dst.y+sp.y, H-1);

    for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
    {
        for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
        {
            vec4 rgba = buf.buf[buf.w*sy + sx];

            uint32_t r = uint32_t(255.0f * rgba.r);
            uint32_t g = uint32_t(255.0f * rgba.g);
            uint32_t b = uint32_t(255.0f * rgba.b);

            backbuffer[W*y + x] = (255<<24) + (r<<16) + (g<<8) + (b<<0);
        }
    }
}
