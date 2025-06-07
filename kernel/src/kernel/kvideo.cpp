#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kassert.h>
#include <kernel/kvideo.hpp>
#include <kernel/bitmanip.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>

#include <driver/video.hpp>

#include <cringe/font.hpp>
#include <bmp.hpp>


#include <cpu/cpu.hpp>
#include <kernel/boot_limine.hpp>
#include <kernel/log.hpp>
#include <kmalloc.h>
#include <kthread.hpp>
#include <kmemxx.hpp>
#include <string.h>
#include <algorithm>


ivec2  kvideo::CSR = ivec2(0, 0);
int    kvideo::W = 0;
int    kvideo::H = 0;
int    kvideo::pitch = 0;
size_t kvideo::BPP = 0;
size_t kvideo::nbytes = 0;

uint8_t *kvideo::frontbuffer;
uint8_t *kvideo::backbuffer;


void kvideo::initFrontbuffer( uintptr_t fbres )
{
    auto *res = (limine_framebuffer_response*)fbres;

    syslog log("kvideo::initFrontbuffer");
    auto *fb = res->framebuffers[0];

    log("fb count:    %u",    res->framebuffer_count);
    log("width:       %u",    fb->width);
    log("height:      %u",    fb->height);
    log("pitch:       %u",    fb->pitch);
    log("bpp:         %u",    fb->bpp);
    log("mem model:   %u",    fb->memory_model);
    log("edid:        0x%lx", fb->edid);
    log("edid_size:   %lu",   fb->edid_size);
    log("mode_count:  %lu",   fb->mode_count);
    log("rmask_size:  %u",    fb->red_mask_size);
    log("rmask_shift: %u",    fb->red_mask_shift);
    log("gmask_size:  %u",    fb->green_mask_size);
    log("gmask_shift: %u",    fb->green_mask_shift);
    log("bmask_size:  %u",    fb->blue_mask_size);
    log("bmask_shift: %u",    fb->blue_mask_shift);

    // {
    //     auto *edid = (VideoEDID*)(fb->edid);
    //     // lg("hSizeMax            %u", edid->hSizeMax);
    //     // lg("vSizeMax            %u", edid->vSizeMax);
    //     // lg("");
    
    //     for (int i=0; i<4; i++)
    //     {
    //         syslog lg("Timing Desc %d", i);
    //         auto &desc = edid->detailedTimingDesc[i];
    //         lg("hFreq:              %u", desc.hFreq);
    //         lg("vFreq:              %u", desc.vFreq);
    //         lg("hActiveTime:        %u", desc.hActiveTime);
    //         lg("hBlankTime:         %u", desc.hBlankTime);
    //         lg("hActiveBlankTime:   %u", desc.hActiveBlankTime);
    //         lg("vActiveTime:        %u", desc.vActiveTime);
    //         lg("vBlankTime:         %u", desc.vBlankTime);
    //         lg("vActiveBlankTime:   %u", desc.vActiveBlankTime);
    //         lg("hSyncOffset:        %u", desc.hSyncOffset);
    //         lg("hSyncPulseW:        %u", desc.hSyncPulseW);
    //         lg("vSyncOffsetPulseW:  %u", desc.vSyncOffsetPulseW);
    //         lg("vhSyncOffsetPulseW: %u", desc.vhSyncOffsetPulseW);
    //         lg("hImageSize:         %u", desc.hImageSize);
    //         lg("vImageSize:         %u", desc.vImageSize);
    //         lg("hvRatio:            %u", desc.hvRatio);
    //         lg("hBorder:            %u", desc.hBorder);
    //         lg("vBorder:            %u", desc.vBorder);
    
    //         {
    //             syslog lg0("Display Type");
    //             auto &disp = desc.displayType;
    //             lg0("unused:       %u", disp.unused);
    //             lg0("syncLocation: %u", disp.syncLocation);
    //             lg0("serrate:      %u", disp.serrate);
    //             lg0("syncType:     %u", disp.syncType);
    //             lg0("stereoMode:   %u", disp.stereoMode);
    //             lg0("interlaced:   %u", disp.interlaced);
    //         }
    //     }
    // }

    kvideo::W      = fb->width;
    kvideo::H      = fb->height;
    kvideo::pitch  = fb->pitch;
    kvideo::BPP    = fb->bpp;
    kvideo::nbytes = pitch*H;
    kvideo::frontbuffer = (uint8_t*)fb->address;
}

void kvideo::initBackbuffer( uintptr_t )
{
    syslog log("kvideo::initBackbuffer");

    {
        using namespace VMM;

        // backbuffer = (uint8_t*)VMM::alloc(nbytes, PAGE_PRESENT|PAGE_WRITE);
        // backbuffer = (uint8_t*)VMM::alloc(nbytes, PAGE_PRESENT|PAGE_WRITE|PAGE_PCD);
        backbuffer = (uint8_t*)kmalloc(nbytes);
    }

    // else
    // {
    //     kvideo::backbuffer = (uint8_t*)kmalloc(kvideo::nbytes);
    // }
}


void kvideo::swapBuffers()
{
    // movsl(frontbuffer, backbuffer, nbytes/4);

    // CPU::movs64(frontbuffer, backbuffer, nbytes/sizeof(uint64_t));
    kmemcpy<uint128_t>(frontbuffer, backbuffer, nbytes);
    kmemset<uint128_t>(backbuffer, 0, nbytes);
}



void kvideo::fill( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
{
    uint8_t rgba[4] = {r, g, b, a};

    int bypp = kvideo::BPP/8;

    for (int i=0; i<kvideo::H; i++)
    {
        for (int j=0; j<kvideo::W; j++)
        {
            int idx = i*kvideo::pitch + bypp*j;

            for (int k=0; k<bypp; k++)
            {
                kvideo::frontbuffer[idx+k] = rgba[k];
            }
        }
    }

    // for (int i=0; i<W*H; i++)
    //     for (size_t j=0; j<stride; j++)
    //         buffer[stride*i + j] = kvideoFillColor[j];
}
