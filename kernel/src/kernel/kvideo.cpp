#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kassert.h>
#include <kernel/kvideo.hpp>
#include <kernel/bitmanip.hpp>

#include <cringe/font.hpp>
#include <cringe/bmp.hpp>


#include <cpu/cpu.hpp>
#include <kernel/boot_limine.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>
#include <kthread.hpp>
#include <kmemxx.hpp>
#include <string.h>
#include <algorithm>


static guiFont kvideoFont(0, 0, 0, 0);
static uint8_t kvideoFillColor[4];

ivec2  kvideo::CSR = ivec2(0, 0);
int    kvideo::W = 0;
int    kvideo::H = 0;
int    kvideo::pitch = 0;
size_t kvideo::BPP = 0;
size_t kvideo::stride = 0;
size_t kvideo::nbytes = 0;

uint8_t *kvideo::frontbuffer;
uint8_t *kvideo::backbuffer;


void kvideo::initFrontbuffer( uintptr_t fbres )
{
    auto *res = (limine_framebuffer_response*)fbres;

    syslog log("kvideo::initFrontbuffer");
    auto *fb = res->framebuffers[0];

    log("fb count:    %u", res->framebuffer_count);
    log("width:       %u", fb->width);
    log("height:      %u", fb->height);
    log("pitch:       %u", fb->pitch);
    log("bpp:         %u", fb->bpp);
    log("pitch/w:     %u", fb->pitch / fb->width);
    log("bpp/8:       %u", fb->bpp / 8);
    log("mem model:   %u", fb->memory_model);
    log("edid:        0x%lx", fb->edid);
    log("rmask_size:  %u", fb->red_mask_size);
    log("rmask_shift: %u", fb->red_mask_shift);
    log("gmask_size:  %u", fb->green_mask_size);
    log("gmask_shift: %u", fb->green_mask_shift);
    log("bmask_size:  %u", fb->blue_mask_size);
    log("bmask_shift: %u", fb->blue_mask_shift);

    kvideo::W      = fb->width;
    kvideo::H      = fb->height;
    kvideo::pitch  = fb->pitch;
    kvideo::BPP    = fb->bpp;
    kvideo::stride = pitch/W;
    kvideo::nbytes = pitch*H;

    kvideo::frontbuffer = (uint8_t*)fb->address;
    kmemset<uint8_t>(kvideoFillColor, 0, sizeof(kvideoFillColor));
}

void kvideo::initBackbuffer( uintptr_t )
{
    kvideo::backbuffer = (uint8_t*)kmalloc(kvideo::nbytes);
}


void kvideo::blit( const ivec2 &dsttl, uint8_t *img, int imgw, int imgh,
                   const ivec2 &srctl, const ivec2 &sp )
{
    uint8_t *dst = kvideo::backbuffer;
    uint8_t *src = img;

    for (int i=0; i<sp.y; i++)
    {
        int dsty = (dsttl.y + i) % kvideo::H;
        int srcy = (srctl.y + i) % imgh;

        for (int j=0; j<sp.x; j++)
        {
            int dstx = (dsttl.x + j) % kvideo::W;
            int srcx = (srctl.x + j) % imgw;

            int dstidx = 4 * (kvideo::W*dsty + dstx);
            int srcidx = 4 * (imgw*srcy + srcx);

            uint8_t alpha = src[srcidx+3];
            for (int k=0; k<4; k++)
                dst[dstidx+k] = ((255-alpha)*dst[dstidx+k] + alpha*src[srcidx+k]) / 255;
            // dst[dstidx+k] = src[srcidx+k];
        }
    
    }  
}


static void kvideo_blit( int x, int y, uint8_t *img, int imgw, int,
                         const ivec2 &tl, const ivec2 &sp )
{
    uint8_t *dst = kvideo::backbuffer;
    uint8_t *src = img;

    int xmin = x;
    int xmax = x + sp.x;

    int ymin = y;
    int ymax = y + sp.y;


    for (int i=0; i<ymax-ymin; i++)
    {
        int dsty = y + i;
        int srcy = tl.y + i;

        for (int j=0; j<xmax-xmin; j++)
        {
            int dstx = x + j;
            int srcx = tl.x + j;

            int dstidx = kvideo::W*4*dsty + 4*dstx;
            int srcidx = imgw*4*srcy + 4*srcx;

            // vec4 dstColor(0.0f);
            // vec4 srcColor(0.0f);

            uint8_t alpha = src[srcidx+3];
            for (int k=0; k<4; k++)
            {
                // dst[dstidx+k] = dst[dstidx+k]*(255-alpha) + src[srcidx+k];
                dst[dstidx+k] = ((255-alpha)*dst[dstidx+k] + alpha*src[srcidx+k]) / 255;

                // dstColor[k] = float(dst[dstidx+k]) / 255.0f;
                // srcColor[k] = float(src[srcidx+k]) / 255.0f;
                // dst[dstidx+k] = src[srcidx+k];
            }
        
            // dstColor = srcColor + (dstColor * (1.0f - srcColor.a));
        
            // for (int k=0; k<4; k++)
            // {
            //     dst[dstidx+k] = uint8_t(255.0f * dstColor[k]);
            // }
        }
    }
}


void kvideo::setFont( const guiFont &font )
{
    kvideoFont = font;
}


void kvideo::renderGlyph( char ch, int x, int y )
{
    kassert(kvideoFont.w != 0 && kvideoFont.h != 0);
    auto &font = kvideoFont;

    ivec2 tl = font.getGlyphCorner(ch);
    ivec2 sp = font.getGlyphExtents();

    kvideo_blit(x, y, font.pixels, font.w, font.h, tl, sp);
}


static void nextLine( const guiContainer &bounds, int &x, int &y, const ivec2 &sp )
{
    int xmin = bounds.xmin();
    int ymin = bounds.ymin();
    int ymax = bounds.ymax();


    x = xmin;
    y += sp.y;

    if (y >= ymax)
        y = ymin;
}


ivec2 kvideo::renderString( const guiContainer &bounds, const char *str, ivec2 csr )
{
    kassert(kvideoFont.w != 0 && kvideoFont.h != 0);

    auto &font = kvideoFont;
    ivec2 sp  = font.getGlyphExtents();
    int   &x   = csr.x;
    int   &y   = csr.y;

    int xmin = bounds.xmin();
    int xmax = bounds.xmax();
    int ymin = bounds.ymin();
    int ymax = bounds.ymax();

    x = std::clamp(x, xmin, xmax);
    y = std::clamp(y, ymin, ymax);

    while (*str)
    {
        char ch = *(str++);

        if (ch == '\n')
            nextLine(bounds, x, y, sp);

        if (x+sp.x >= xmax)
            nextLine(bounds, x, y, sp);

        ivec2 tl = font.getGlyphCorner(ch);
        if (tl.x == -1)
            continue;

        kvideo::blit(csr, font.pixels, font.w, font.h, tl, sp);

        x += sp.x;
    }

    return ivec2(x, y);
}


ivec2 kvideo::renderString( const char *str, ivec2 csr )
{
    return kvideo::renderString({ivec2(0), ivec2(kvideo::W, kvideo::H)}, str, csr);
}


// void kvideo::cursorString( const char *str )
// {
//     kassert(kvideoFont.W != 0 && kvideoFont.H != 0);
//     auto &font = kvideoFont;

//     ivec2 sp  = font.getGlyphExtents();
//     int  &x = kvideo::CSR.x;
//     int  &y = kvideo::CSR.y;

//     while (*str)
//     {
//         char ch = *(str++);

//         if (ch == '\n')
//             nextLine(x, y, sp);

//         if (x+sp.x >= kvideo::W)
//             nextLine(x, y, sp);

//         ivec2 tl = font.getGlyphCorner(ch);
//         if (tl.x == -1)
//             continue;

//         kvideo::blit(ivec2(x, y), font.m_img, font.W, font.H, tl, sp);

//         x += sp.x;
//     }
// }



void kvideo::swapBuffers()
{
    // kmemcpy<uint8_t>(frontbuffer, backbuffer, nbytes);
    // kmemset<uint8_t>(backbuffer, 0, nbytes);
    uint8_t *A = idk::align_up(frontbuffer, 16);\
    uint8_t *B = idk::align_up(backbuffer, 16);

    kmemcpy<uint128_t>(A, B, nbytes);
    kmemset<uint128_t>(B, 0, nbytes);


}



static void bufferCheck( uint8_t *buffer )
{
    if ((buffer != kvideo::backbuffer) && (buffer != kvideo::frontbuffer))
    {
        syslog::println("[kvideo::clearBuffer] wtf??");
        CPU::hcf();
    }
}


void kvideo::clearBuffer( uint8_t *buffer )
{
    bufferCheck(buffer);
    kmemset<uint8_t>(buffer, 0, nbytes);
}


void kvideo::fillColor( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
{
    kvideoFillColor[0] = b;
    kvideoFillColor[1] = g;
    kvideoFillColor[2] = r;
    kvideoFillColor[3] = a;
}


void kvideo::fillBuffer( uint8_t *buffer )
{
    bufferCheck(buffer);

    for (int i=0; i<W*H; i++)
        for (size_t j=0; j<stride; j++)
            buffer[stride*i + j] = kvideoFillColor[j];
}



void kvideo::rect( int x0, int y0, int w, int h )
{
    for (int y=y0; y<y0+h; y++)
    {
        if (y<0 || y>=kvideo::H)
            continue;

        for (int x=x0; x<x0+w; x++)
        {
            if (x<0 || x>=kvideo::W)
                continue;

            size_t idx = stride * (W*y + x);
            for (size_t k=0; k<stride; k++)
                backbuffer[idx + k] = kvideoFillColor[k];
        }
    }
}


