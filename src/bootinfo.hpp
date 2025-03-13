#pragma once

#define LIMINE_API_REVISION 3
#include <../../external/limine/limine.h>
// #include <limine.h>


struct ckBootInfo
{
    uint64_t fb_count;
    limine_framebuffer **fb_list;
    limine_memmap_response *mmap;
};

