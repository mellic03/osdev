#pragma once
#include <AAA.h>


struct Kmemmap_t
{
    uint64_t addr;
    uint64_t len;
};


void *kmalloc( size_t size );
void  kfree( void* );


#include <AAB.h>
