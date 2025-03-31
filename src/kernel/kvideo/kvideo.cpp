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

    syslog log("idk::Video::init");
    auto *fb = res->framebuffers[0];

    kvideo::W = fb->width;
    kvideo::H = fb->height;

    kvideo::frontbuffer = (uint32_t*)fb->address;
    kvideo::backbuffer  = (uint32_t*)kmalloc(W*H*sizeof(uint32_t));
}


void kvideo::swapBuffers()
{
    kmemcpy<uint128_t>(frontbuffer, backbuffer, W*H*sizeof(uint32_t));
    kmemset<uint128_t>(backbuffer, 0, W*H*sizeof(uint32_t));
}



void kvideo::fill( uint32_t C )
{
    __uint128_t color = (__uint128_t(C)<<96) + (__uint128_t(C)<<64)
                      + (__uint128_t(C)<<32) +  __uint128_t(C);

    kmemset<uint128_t>(backbuffer, color, W*H*sizeof(uint32_t));

    // for (int i=0; i<(W*H)/4; i++)
    // {
    //     *(buf++) = color;
    // }
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
            // if (srcbuf[sy][sx].a > 0.1f)
            dstbuf[y][x] = srcbuf[sy][sx];
        }
    }
}


void kvideo::blit( ivec2 dst, ivec2 src, ivec2 sp,
                   const kframebuffer<vec4> &buf )
{
    int xmin = std::max(dst.x, 0);
    int xmax = std::min(dst.x+sp.x, W-1);
    int ymin = std::max(dst.y, 0);
    int ymax = std::min(dst.y+sp.y, H-1);

    for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
    {
        for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
        {
            vec4 rgba = buf[sy][sx];

            uint32_t r = uint32_t(255.0f * rgba.r);
            uint32_t g = uint32_t(255.0f * rgba.g);
            uint32_t b = uint32_t(255.0f * rgba.b);

            backbuffer[W*y + x] = (255<<24) + (r<<16) + (g<<8) + (b<<0);
        }
    }
}