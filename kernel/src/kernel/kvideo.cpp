#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kernel/kvideo.hpp>
#include "kvideo.hpp"

#include <kernel/boot_limine.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>
#include <khang.h>
#include <kthread.hpp>
#include <kmemxx.hpp>
#include <string.h>
#include <algorithm>



static uint8_t kvideoFillColor[4];
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

    syslog log("kvideo::init");
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


void kvideo::swapBuffers()
{
    kmemcpy<uint8_t>(frontbuffer, backbuffer, nbytes);
    kmemset<uint8_t>(backbuffer, 0, nbytes);
}




static void bufferCheck( uint8_t *buffer )
{
    if ((buffer != kvideo::backbuffer) && (buffer != kvideo::frontbuffer))
    {
        syslog::println("[kvideo::clearBuffer] wtf??");
        kernel::hang();
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


