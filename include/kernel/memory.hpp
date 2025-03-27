#pragma once

#include <stddef.h>
#include <stdint.h>
#include "linear_allocator.hpp"
#include "buddy_allocator.hpp"

inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() { };


namespace idk
{
    namespace memory
    {
        extern uint64_t hhdm;

        inline uint64_t asphysical( uint64_t vaddr )
        {
            return vaddr - hhdm;
        }
    
        inline uint64_t asvirtual( uint64_t paddr )
        {
            return paddr + hhdm;
        }
        
    }
 
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

