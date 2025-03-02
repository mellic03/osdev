#pragma once
#include <../../external/limine-9.x/limine.h>


struct ckBootInfo
{
    uint64_t fb_count;
    limine_framebuffer **fb_list;
    limine_memmap_response *mmap;
    limine_smp_response     smp;
};

