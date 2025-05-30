#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kassert.h>
#include <kernel/kvideo.hpp>
#include <kernel/bitmanip.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>

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

    log("fb count:    %u", res->framebuffer_count);
    log("width:       %u", fb->width);
    log("height:      %u", fb->height);
    log("pitch:       %u", fb->pitch);
    log("bpp:         %u", fb->bpp);
    log("mem model:   %u", fb->memory_model);
    log("edid:        0x%lx", fb->edid);
    log("edid_size:   %lu",   fb->edid_size);
    log("mode_count:  %lu",   fb->mode_count);
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
    kvideo::nbytes = pitch*H;
    log("size:        %lu KB", kvideo::nbytes/1000);

    kvideo::frontbuffer = (uint8_t*)fb->address;
}

void kvideo::initBackbuffer( uintptr_t )
{
    syslog log("kvideo::initBackbuffer");

    // if (kvideo::nbytes < 3*PMM::PAGE_SIZE)
    // {
    //     using namespace VMM;
    
    //     uintptr_t phys1 = PMM::alloc();
    //     uintptr_t phys2 = PMM::alloc();
    //     uintptr_t phys3 = PMM::alloc();
    //     uintptr_t virt  = phys1 + PMM::hhdm;

    //     VMM::mapPage(phys1, virt+0*PMM::PAGE_SIZE, PAGE_PRESENT|PAGE_WRITE|PAGE_PWT);
    //     VMM::mapPage(phys2, virt+1*PMM::PAGE_SIZE, PAGE_PRESENT|PAGE_WRITE|PAGE_PWT);
    //     VMM::mapPage(phys3, virt+2*PMM::PAGE_SIZE, PAGE_PRESENT|PAGE_WRITE|PAGE_PWT);
    
    //     kvideo::backbuffer = (uint8_t*)virt;
    // }

    // else
    // {
        kvideo::backbuffer = (uint8_t*)kmalloc(kvideo::nbytes);
    // }
}


static inline void movsd( void *dst, const void *src, size_t size )
{
    asm volatile("rep movsl" : "+D"(dst), "+S"(src), "+c"(size) : : "memory");
}

void kvideo::swapBuffers()
{
    movsd(frontbuffer, backbuffer, nbytes/4);
    // kmemcpy<uint8_t>(frontbuffer, backbuffer, nbytes);
    kmemset<uint8_t>(backbuffer, 0, nbytes);

    // uint8_t *A = idk::align_up(frontbuffer, 16);
    // uint8_t *B = idk::align_up(backbuffer, 16);
    // kmemcpy<uint128_t>(A, B, nbytes);
    // kmemset<uint128_t>(B, 0, nbytes);
}



void kvideo::fill( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
{
    uint8_t rgba[4] = {r, g, b, a};

    for (int i=0; i<kvideo::H; i++)
    {
        for (int j=0; j<kvideo::W; j++)
        {
            int idx = i*kvideo::pitch + j;

            for (size_t k=0; k<kvideo::BPP/8; k++)
                kvideo::frontbuffer[idx+k] = rgba[k];
        }
    }

    // for (int i=0; i<W*H; i++)
    //     for (size_t j=0; j<stride; j++)
    //         buffer[stride*i + j] = kvideoFillColor[j];
}
