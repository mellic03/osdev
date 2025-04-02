#pragma once
#include <AAA.h>

void  kmalloc_init( uintptr_t base, size_t size );
void *kmalloc( size_t size );
// void *kmallocAligned( size_t size, size_t alignment );
void *krealloc( void*, size_t size );
void  kfree( void* );


#include <ZZZ.h>
