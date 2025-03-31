#pragma once
#include <AAA.h>

#ifdef __is_kernel
    #include <kernel/memory.hpp>

    void  kmalloc_init( uintptr_t base, size_t size );
    void *kmalloc( size_t size );
    // void *kmallocAligned( size_t size, size_t alignment );
    void *krealloc( void*, size_t size );
    void  kfree( void* );
#endif


#include <ZZZ.h>
