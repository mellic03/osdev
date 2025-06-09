#pragma once

#ifndef LIMINE_API_REVISION
    #define LIMINE_API_REVISION 3
#endif

#include <limine/limine.h>

struct LimineRes
{
    uint64_t                         hhdm;
    limine_framebuffer_response     *fb;
    limine_module_response          *modules;
    limine_memmap_response          *mmaps;
    limine_executable_file_response *fh;
    limine_executable_address_response *execaddr;
    limine_mp_response              *mp;
};

extern LimineRes limine_res;
