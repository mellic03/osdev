#pragma once
#include <stdint.h>


struct tss_t
{
    uint32_t reserved __attribute__((aligned(16)));
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved2;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved3;
    uint32_t reserved4;
    uint32_t iopbOffset;
} __attribute__((packed));


struct gdt_ptr_t
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));


namespace GDT
{
    void load();
}