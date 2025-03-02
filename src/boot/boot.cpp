// https://github.com/limine-bootloader/limine/blob/49f4ccd3122575be023478910176777b6d09a97d/PROTOCOL.md#x86_64-2

#include "bootinfo.hpp"
extern void kmain( ckBootInfo* );



// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.
__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;


__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request mmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
    .response = nullptr,
    .flags = 0
};


__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;




void _start()
{
    // // Ensure the bootloader actually understands our base revision (see spec).
    // if (LIMINE_BASE_REVISION_SUPPORTED == false)
    // {
    //     hcf();
    // }

    // // Ensure we got a framebuffer.
    // if (framebuffer_request.response == NULL
    //  || framebuffer_request.response->framebuffer_count < 1) {
    //     hcf();
    // }

    auto *fb_res = framebuffer_request.response;

    ckBootInfo info = {
        .fb_count = fb_res->framebuffer_count,
        .fb_list  = framebuffer_request.response->framebuffers,
        .mmap     = mmap_request.response,
        .smp      = *(smp_request.response)
    };

    // info.smp.cpus[0]->goto_address

    kmain(&info);

    // // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    // for (size_t i = 0; i < 100; i++)
    // {
    //     uint32_t *fb_ptr = static_cast<uint32_t*>(framebuffer_request.response->framebuffers[0]->address);
    //     fb_ptr[i * (framebuffer_request.response->framebuffers[0]->pitch / 4) + i] = 0xffffff;
    // }

    // // We're done, just hang...
    // hcf();
}
