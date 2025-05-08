#pragma once

#include <stdint.h>
#include <stddef.h>
#include <kernel/memory.hpp>

#define PMM_2MB_PAGES


namespace PMM
{
    #ifdef PMM_2MB_PAGES
        static constexpr size_t PAGE_SIZE = 2*idk::MEGA; 
    #else
        static constexpr size_t PAGE_SIZE = 4*idk::KILO; 
    #endif

    extern uint64_t hhdm;
    struct MemMap { uintptr_t base; size_t size; };

    void init( MemMap *mmaps, size_t num_mmaps, size_t hhdm_offset );
    uintptr_t alloc();
    void free( uintptr_t phys );

}
