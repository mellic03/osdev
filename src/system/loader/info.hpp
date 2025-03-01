#pragma once

#include <stddef.h>
#include <stdint.h>
#include "../kcommon/memory/stack_allocator.hpp"


struct ckMemblock
{
    uint32_t *start;
    uint32_t *end;
    uint32_t  len;

    ckMemblock() {  };

    ckMemblock( uint32_t a, uint32_t b )
    {
        start = (uint32_t*)(a);
        end   = (uint32_t*)(b);
        len   = (uint32_t)(end - start);
    }
};

struct ckMemoryInfo
{
    ck::linear_allocator memblock;
    ck::stack_allocator  stack;

    uint32_t   total_size;
    uint32_t   num_mmaps;
    ckMemblock mmaps[32];
};


struct ckModule
{
    uint32_t *start;
    uint32_t *end;
};


// struct ckModuleInfo
// {
//     ckModule kernel_x64;
//     ckModule font;
// };


struct ckInfo
{
    ckMemoryInfo memory;
    // ckModuleInfo modules;

    uint32_t  font;
};


struct multiboot_tag;


ckInfo  ckInfoLoad( uint32_t *addr );

