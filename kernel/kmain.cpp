#include <cxxabiv1>
#include <idk_kernel.h>
#include <idk_hcf.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define LIMINE_API_REVISION 3
#include <limine/limine.h>


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



#include "libk_init.h"
#include "libc_init.h"
#include "libc++_init.hpp"
#include <kdriver/serial.hpp>
#include <kernel/cpu.hpp>
#include "./filesystem/filesystem.hpp"






extern "C"
void _start()
{
    idk::CPU cpu;
    cpu.enableSSE();

    auto hhdm     = hhdm_req.response;
    // auto fb_count = fb_req.response->framebuffer_count;
    // auto fb_list  = fb_req.response->framebuffers;
    // auto dtb      = dtb_req.response;
    // auto modules  = module_req.response;
    // auto mmap     = mmap_req.response;
    // auto mp       = mp_req.response;


    if (!idk::serial_init()) // KRET_FAILURE
    {
        idk_hcf();
        return;
    }

    SYSLOG_BEGIN("_start");
    SYSLOG("serial initialized");


    kmalloc_init(hhdm->offset, (uint64_t)(mmap_req.response));
    kvirtio_init();
    kfilesystem_init();



    libc_init();
    // libcpp_init();

    // int libc_ret   = libc_init();
    // int libcpp_ret = libcpp_init();


    SYSLOG_END();
    idk_hcf();
}


