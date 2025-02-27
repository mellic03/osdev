#pragma once
#include "stack_allocator.hpp"

struct ckMemoryInfo
{
    ck::linear_allocator mainblock;
    ck::stack_allocator  stack;

    uint32_t mmap_count;
    uint32_t mmap_addr[64];
    uint32_t mmap_size[64];
};



int ckMemoryInit( unsigned long, ckMemoryInfo* );

