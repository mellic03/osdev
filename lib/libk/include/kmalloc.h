#pragma once
#include <kernel/memory.hpp>

#include <AAA.h>



void  kmalloc_init( const idk::MemoryMap& );
void *kmalloc( size_t size );
void  kfree( void* );

#include <ZZZ.h>


#ifdef __cplusplus

    template <typename T>
    inline constexpr
    T *KMalloc( size_t n )
    {
        return (T*)kmalloc(n*sizeof(T));
    }

#endif


