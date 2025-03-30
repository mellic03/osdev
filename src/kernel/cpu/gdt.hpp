#pragma once
#include <stddef.h>
#include <stdint.h>


static constexpr uint16_t GDT_OFFSET_KERNEL_CODE = 0x08;
static constexpr uint16_t GDT_OFFSET_KERNEL_DATA = 0x10;


struct gdtr_t
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));


struct gdt_entry_t
{
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_hi;
} __attribute__((packed));
  



struct tss_entry_t
{
    uint32_t reserved1;
    uint64_t rsp0; // Stack pointer for ring 0
    uint64_t rsp1; // Stack pointer for ring 1
    uint64_t rsp2; // Stack pointer for ring 2
    uint64_t reserved2;
    uint64_t ist1; // Interrupt Stack Table 1
    uint64_t ist2; // Interrupt Stack Table 2
    uint64_t ist3; // Interrupt Stack Table 3
    uint64_t ist4; // Interrupt Stack Table 4
    uint64_t ist5; // Interrupt Stack Table 5
    uint64_t ist6; // Interrupt Stack Table 6
    uint64_t ist7; // Interrupt Stack Table 7
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t io_map_base; // Offset for I/O permission bitmap
}  __attribute__((packed));


namespace GDT
{
    void load();
    void flush();
}


