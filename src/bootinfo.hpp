#pragma once

#define LIMINE_API_REVISION 3
#include <../../external/limine/limine.h>
// #include <limine.h>

struct idk_BootInfo
{
    limine_hhdm_response   *hhdm;

    uint64_t               fb_count;
    limine_framebuffer   **fb_list;

    limine_dtb_response    *dtb;
    limine_module_response *modules;
    limine_memmap_response *mmap;
    limine_mp_response     *mp;
};
