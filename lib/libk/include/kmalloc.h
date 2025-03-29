#pragma once
#include <AAA.h>
#include <kernel/memory.hpp>



void  kmalloc_init( const idk::MemoryMap& );
void *kmalloc( size_t size );
void  kfree( void* );



#include <ZZZ.h>

