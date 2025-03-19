#pragma once

#include "./linear_allocator.hpp"
#include "./buddy_allocator.hpp"
#include "../../bootinfo.hpp"


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


namespace idk::memory
{
    // idk::base_allocator *init( limine_memmap_response* );
    void init( idk::linear_allocator&, limine_memmap_response*, uint64_t hhdm );

    idk::buddy_allocator &getBuddy();
}



