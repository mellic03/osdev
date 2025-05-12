#pragma once
#include <stdint.h>


struct gdt_ptr_t
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

