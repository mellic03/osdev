extern "C"
{
    #define LIMINE_API_REVISION 3
    #include <limine/limine.h>
}

#include "kvideo.hpp"
#include "../log/log.hpp"
#include <kmalloc.h>
#include <kmemxx.h>
#include <string.h>
#include <algorithm>


using namespace idk;


kframebuffer<uint32_t> kvideo::frontbuffer;
kframebuffer<uint32_t> kvideo::backbuffer;


void kvideo::init( limine_framebuffer_response *res )
{
    syslog log("idk::Video::init");
    auto *fb = res->framebuffers[0];

    int W = fb->width;
    int H = fb->height;

    kvideo::frontbuffer  = kframebuffer<uint32_t>(W, H, (uint32_t*)fb->address);
    kvideo::backbuffer   = kframebuffer<uint32_t>(W, H, (uint32_t*)kmalloc(W*H * sizeof(uint32_t)));
}


void kvideo::swapBuffers()
{
    int W = frontbuffer.w;
    int H = frontbuffer.h;

    kmemcpy<uint32_t>(frontbuffer.buf, backbuffer.buf, W*H);
    kmemset<uint32_t>(backbuffer.buf, 0, W*H);
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
            if (srcbuf[sy][sx].a > 0.1f)
                dstbuf[y][x] = srcbuf[sy][sx];
        }
    }
}


void kvideo::blit( ivec2 dst, ivec2 src, ivec2 sp,
                   const kframebuffer<vec4> &buf )
{

    int xmin = std::max(dst.x, 0);
    int xmax = std::min(dst.x+sp.x, backbuffer.w-1);
    int ymin = std::max(dst.y, 0);
    int ymax = std::min(dst.y+sp.y, backbuffer.h-1);

    for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
    {
        for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
        {
            vec4 argb = buf[sy][sx];

            uint32_t r = uint32_t(255.0f * argb.r);
            uint32_t g = uint32_t(255.0f * argb.g);
            uint32_t b = uint32_t(255.0f * argb.b);

            backbuffer[y][x] = (255<<24) + (r<<16) + (g<<8) + (b<<0);
        }
    }
}