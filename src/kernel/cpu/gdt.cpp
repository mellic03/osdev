#include "./gdt.hpp"
#include "../log/log.hpp"

// Each define here is for a specific flag in the descriptor.
// Refer to the intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)
 
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed
 
#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR
 
#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWR




uint64_t create_descriptor( uint32_t base, uint32_t limit, uint16_t flag )
{
    uint64_t descriptor;
 
    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;
 
    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0
 
    return descriptor;
}


static uint64_t    __gdt[16];
static tss_entry_t __TSS;



static void
fill_entry( int idx, uint64_t base, uint64_t limit, uint8_t access_byte, uint8_t flags )
{
    uint8_t *target = (uint8_t*)(&__gdt[idx]);

    // Encode the limit
    target[0] =  limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] = (limit >> 16) & 0x0F;

    // // Encode the base
    target[2] =  base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // // Encode the access byte
    target[5] = access_byte;

    // // Encode the flags
    target[6] |= (flags << 4);
}



extern "C"
{
    extern uint16_t __gdtr_limit;
    extern uint64_t __gdtr_base;
    extern void __GDT_load(void);
    extern void __GDT_flush(void);
}
 
constexpr uint16_t __gdt_count = 5;

void
idk::GDT_load()
{
    syslog log("GDT_load");

    // https://wiki.osdev.org/GDT_Tutorial#How_to_Set_Up_The_GDT
    fill_entry(0, 0, 0, 0, 0);
    fill_entry(1, 0, 0xFFFFF, 0x9A, 0xA); // Kernel mode code segment
    fill_entry(2, 0, 0xFFFFF, 0x92, 0xC); // Kernel mode data segment
    fill_entry(3, 0xFFFFF, 0xF00000, 0xFA, 0xA); // User mode code segment
    fill_entry(4, 0xFFFFF, 0xF00000, 0xF2, 0xC); // User mode data segment

    for (int i=0; i<__gdt_count; i++)
    {
        log("GDT[%d]: 0x%lx", i, __gdt[i]);
    }

    __gdtr_limit = uint16_t(__gdt_count * sizeof(uint64_t) - 1);
    __gdtr_base  = (uint64_t)(&__gdt[0]);

	log("gdtr.limit: %lu",   __gdtr_limit);
	log("gdtr.base:  0x%lx", __gdtr_base);

    __GDT_load();
}




#include <kernel/memory.hpp>

void
idk::GDT_flush()
{
	syslog log("GDT_flush");
    __GDT_flush();
}
