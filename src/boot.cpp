// https://github.com/limine-bootloader/limine/blob/49f4ccd3122575be023478910176777b6d09a97d/PROTOCOL.md#x86_64-2

#include "bootinfo.hpp"
extern void kmain( const ckBootInfo& );




__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);


__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST
};


__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request mmap_request = {
    .id = LIMINE_MEMMAP_REQUEST
};


__attribute__((used, section(".limine_requests")))
static volatile struct limine_mp_request mp_request = {
    .id = LIMINE_MP_REQUEST
};


__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;





extern "C"
void _start()
{
    auto *fb_res = framebuffer_request.response;

    // mp_request.response->cpu_count

    ckBootInfo info = {
        .fb_count = fb_res->framebuffer_count,
        .fb_list  = framebuffer_request.response->framebuffers,
        .mmap     = mmap_request.response
    };

    // info.smp.cpus[0]->goto_address


    kmain(info);
}
