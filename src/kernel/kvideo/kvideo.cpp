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

    kvideo::frontbuffer = kframebuffer<uint32_t>(W, H, (uint32_t*)fb->address);
    kvideo::backbuffer  = kframebuffer<uint32_t>(W, H, (uint32_t*)kmalloc(W*H * sizeof(uint32_t)));
}


void kvideo::swapBuffers()
{
    int W = frontbuffer.w;
    int H = frontbuffer.h;
    kmemcpy<uint32_t>(frontbuffer.buf, backbuffer.buf, W*H);
    kmemset<uint32_t>(backbuffer.buf, 0, W*H);
}


void kvideo::blit( ivec2 dst, ivec2 src, ivec2 sp,
                   const kframebuffer<uint32_t> &buf )
{
    int xmin = std::max(dst.x, 0);
    int xmax = std::min(dst.x+sp.x, backbuffer.w-1);
    int ymin = std::max(dst.y, 0);
    int ymax = std::min(dst.y+sp.y, backbuffer.h-1);

    for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
    {
        for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
        {
            backbuffer[y][x] = buf[sy][sx];
        }
    }
}