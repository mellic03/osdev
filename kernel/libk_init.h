#pragma once
#include <idk_kernel.h>

kret_t kmalloc_init( uint64_t hhdm_offset, uint64_t mmap_res_addr );
kret_t kvirtio_init();
