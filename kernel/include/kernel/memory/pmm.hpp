#pragma once

#include <stdint.h>
#include <stddef.h>
#include <kernel/memory.hpp>

#define PMM_2MB_PAGES

namespace PMM
{
    static constexpr size_t PAGE_SIZE_4K = 4*1024;
    static constexpr size_t PAGE_SIZE_2M = 2*1024*1024;

    #ifdef PMM_2MB_PAGES
        static constexpr size_t PAGE_SIZE = 2*idk::MEGA; 
    #else
        static constexpr size_t PAGE_SIZE = 4*idk::KILO; 
    #endif

    static constexpr size_t FRAME_SIZE = 512*sizeof(uint64_t);


    extern uint64_t hhdm;
    struct MemMap { uintptr_t base; size_t size; };

    void init( const MemMap &mmap, size_t hhdm_offset );

    uintptr_t allocFrame();
    uintptr_t allocPage();
    uintptr_t allocPages( size_t n );
    void freeFrame( uintptr_t phys );
    void freePage( uintptr_t phys );

}
