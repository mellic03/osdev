#ifndef __is_kernel
    #define __is_kernel
#endif

#include <stddef.h>
#include <stdint.h>

#include <kernel/boot_limine.hpp>
#include <kernel/log.hpp>
#include <driver/serial.hpp>
#include <cpu/cpu.hpp>
extern void early_init();


extern "C"
void _start()
{
    CPU::cli();
    // knl_ClearBSS();
    // knl_ClearMemory();
    CPU::initFoat();

    LimineRes_init();

    CPU::createGDT();
    CPU::installGDT();

    syslog::disable();
    early_init();

    auto *res = limine_res.fb;
    auto *fb  = res->framebuffers[0];
    auto *dst = (uint8_t*)(fb->address);
    size_t nbytes = fb->pitch * fb->height * (fb->bpp / 8);

    if (fb->bpp == 32)
    {
        for (size_t y=0; y<fb->height; y++)
        {
            for (size_t x=0; x<fb->width; x++)
            {
                size_t i = fb->pitch*y + 4*x;
                dst[i+0] = 100;
                dst[i+1] = 255 - (255*y) / fb->height;
                dst[i+2] = (255*x) / fb->width;
                dst[i+3] = 255;
            }
        }
    }

    else
    {
        for (size_t i=0; i<nbytes/4; i++)
        {
            dst[i] = 255;
        }
    }


    while (true)
    {
        asm volatile ("cli; hlt");
    }

}


