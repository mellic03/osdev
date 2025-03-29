#pragma once

#include <stddef.h>
#include <stdint.h>
#include "linear_allocator.hpp"
#include "buddy_allocator.hpp"


namespace idk
{
    static constexpr size_t KILO = 1024;
    static constexpr size_t MEGA = 1024*KILO;
    static constexpr size_t GIGA = 1024*MEGA;

    struct MemoryMap
    {   uint64_t phys;
        uint64_t addr;
        uint64_t len;

        MemoryMap(): MemoryMap(0, 0, 0) {  }
        MemoryMap( uint64_t p, uint64_t l, uint64_t hhdm )
        : phys(p), addr(p+hhdm), len(l) {  }
    };
}

