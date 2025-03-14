// https://github.com/limine-bootloader/limine/blob/49f4ccd3122575be023478910176777b6d09a97d/PROTOCOL.md#x86_64-2

#include "bootinfo.hpp"
void kmain( const idk_BootInfo& );




__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);


__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_req = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request fb_req = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_dtb_request dtb_req = {
    .id = LIMINE_DTB_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_module_request module_req = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request mmap_req = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_mp_request mp_req = {
    .id = LIMINE_MP_REQUEST,
    .revision = 0
};


__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;





extern "C"
{
    #include "system/ctor/icxxabi.h"
    #include "system/ctor/cxxabiv1.hpp"
    

    void _start()
    {
        auto *fb_res = fb_req.response;

        idk_BootInfo info = {
            .hhdm     = hhdm_req.response,
            .fb_count = fb_res->framebuffer_count,
            .fb_list  = fb_req.response->framebuffers,
            .dtb      = dtb_req.response,
            .modules  = module_req.response,
            .mmap     = mmap_req.response,
            .mp       = mp_req.response
        };


        kmain(info);
    }
}
