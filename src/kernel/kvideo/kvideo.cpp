#ifndef __is_kernel
    #define __is_kernel
#endif

#include "kvideo.hpp"
#include "../boot/boot.hpp"
#include <kernel/log.hpp>
#include <kernel/vfs.hpp>
#include <kmalloc.h>
#include <kthread.hpp>
#include <kmemxx.h>
#include <string.h>
#include <algorithm>


int kvideo::W = 512;
int kvideo::H = 512;

uint32_t *kvideo::frontbuffer;
uint32_t *kvideo::backbuffer;


void kvideo::init( uintptr_t fbres )
{
    auto *res = (limine_framebuffer_response*)fbres;
    char buf[128];
    memset(buf, 0, 128);

    for (size_t i=0; i<res->framebuffer_count; i++)
    {
        sprintf(buf, "/dev/fb%d", int(i));
        auto *fh = vfsInsertFile<char>(buf, 256, vfsFileFlag_Text);
        fh->flags |= vfsFileFlag_Text;
        sprintf((char*)(fh->addr), "test content 1\ntest content 2\n");
    }

    syslog log("kvideo::init");
    auto *fb = res->framebuffers[0];

    kvideo::W = fb->width;
    kvideo::H = fb->height;

    kvideo::frontbuffer = (uint32_t*)fb->address;
    kvideo::backbuffer  = (uint32_t*)kmalloc(W*H*sizeof(uint32_t));
}

void kvideo::swapBuffers()
{
    kmemcpy<uint32_t>(frontbuffer, backbuffer, W*H*sizeof(uint32_t));
}



void kvideo::fill( uint32_t C )
{
    __uint128_t color = (__uint128_t(C)<<96) + (__uint128_t(C)<<64)
                      + (__uint128_t(C)<<32) +  __uint128_t(C);

    kmemset<uint128_t>(backbuffer, color, W*H);
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


// void kvideo::blit( ivec2 dst, ivec2 src, ivec2 sp,
//                    kframebuffer<vec4> &dstbuf,
//                    const kframebuffer<vec4> &srcbuf )
// {
//     int xmin = std::max(dst.x, 0);
//     int xmax = std::min(dst.x+sp.x, dstbuf.w-1);
//     int ymin = std::max(dst.y, 0);
//     int ymax = std::min(dst.y+sp.y, dstbuf.h-1);

//     for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
//     {
//         for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
//         {
//             dstbuf[y][x] = srcbuf[sy][sx];
//         }
//     }

//     // int dx = std::min(xmax-xmin, sp.x);

//     // for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
//     // {
//     //     kmemcpy<uint128_t>(dstbuf[y], srcbuf[sy], dx*sizeof(vec4));
//     // }
// }


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

// void kvideo::blit( ivec2 dst, ivec2 src, ivec2 sp, const vec4 *buf, int bufw )
// {
//     using namespace kvideo;

//     int xmin = std::max(dst.x, 0);
//     int xmax = std::min(dst.x+sp.x, W-1);
//     int ymin = std::max(dst.y, 0);
//     int ymax = std::min(dst.y+sp.y, H-1);

//     for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
//     {
//         for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
//         {
//             vec4 rgba = buf[bufw*sy + sx];

//             uint32_t r = uint32_t(255.0f * rgba.r);
//             uint32_t g = uint32_t(255.0f * rgba.g);
//             uint32_t b = uint32_t(255.0f * rgba.b);

//             backbuffer[W*y + x] = (255<<24) + (r<<16) + (g<<8) + (b<<0);
//         }
//     }
// }

// void kvideo::blit( ivec2 dst, ivec2 src, ivec2 sp, const uint32_t *buf, int bufw )
// {
//     using namespace kvideo;

//     int xmin = std::max(dst.x, 0);
//     int xmax = std::min(dst.x+sp.x, W-1);
//     int ymin = std::max(dst.y, 0);
//     int ymax = std::min(dst.y+sp.y, H-1);

//     // int dx = std::min(xmax-xmin, sp.x);
//     src.x = std::clamp(src.x, 0, W-1);
//     src.y = std::clamp(src.y, 0, H-1);

//     for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
//     {
//         // int offset1 = W*y + xmin;
//         // int offset2 = bufw*sy + src.x;
//         // kmemcpy<uint128_t>(backbuffer+offset1, buf+offset2, (xmax-xmin) * sizeof(uint32_t));

//         for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
//         {
//             uint32_t s = buf[bufw*sy + sx];
//             uint32_t d = backbuffer[W*y + x];

//             uint8_t  a = uint8_t(s >> 24);
//             uint8_t sr = uint8_t(s >> 16);
//             uint8_t sg = uint8_t(s >> 8);
//             uint8_t sb = uint8_t(s >> 0);

//             uint8_t da = uint8_t(s >> 24);
//             uint8_t dr = uint8_t(d >> 16);
//             uint8_t dg = uint8_t(d >> 8);
//             uint8_t db = uint8_t(d >> 0);

//             da = (sr*a + da * (255-a)) / 255;
//             dr = (sr*a + dr * (255-a)) / 255;
//             dg = (sg*a + dg * (255-a)) / 255;
//             db = (sb*a + db * (255-a)) / 255;

//             backbuffer[W*y + x] = (da<<24) + (dr<<16) + (dg<<8) + (db);

//             // backbuffer[W*y + x] = buf[bufw*sy + sx];
//         }
//     }
// }
