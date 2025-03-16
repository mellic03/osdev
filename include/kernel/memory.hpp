#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kernel/linear_allocator.hpp>
#include <kernel/buddy_allocator.hpp>


void *operator new( size_t );
void *operator new[]( size_t );
void  operator delete( void* );
void  operator delete[]( void* );
void  operator delete( void*, size_t );
void  operator delete[]( void*, size_t );

inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() { };


namespace idk
{
    static constexpr size_t KILO = 1024;
    static constexpr size_t MEGA = 1024*KILO;
    static constexpr size_t GIGA = 1024*MEGA;

    struct MemoryMap
    {
        uint64_t addr;
        uint64_t len;

        MemoryMap(): MemoryMap(0, 0) {  }
        MemoryMap( uint64_t a, uint64_t l )
        : addr(a), len(l) {  }
    };
}

