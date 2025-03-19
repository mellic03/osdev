#include "./gdt.hpp"

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

struct tss_entry
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
};


struct gdt_tss
{
	uint16_t previous_task, __previous_task_unused;
	uint32_t esp0;
	uint16_t ss0, __ss0_unused;
	uint32_t esp1;
	uint16_t ss1, __ss1_unused;
	uint32_t esp2;
	uint16_t ss2, __ss2_unused;
	uint32_t cr3;
	uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	uint16_t es, __es_unused;
	uint16_t cs, __cs_unused;
	uint16_t ss, __ss_unused;
	uint16_t ds, __ds_unused;
	uint16_t fs, __fs_unused;
	uint16_t gs, __gs_unused;
	uint16_t ldt_selector, __ldt_sel_unused;
	uint16_t debug_flag, io_map;
} __attribute__ ((packed));



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
 

static uint64_t gdt_entries[16];
static idk_GDTR example_gdtr;


void
idk::sys::loadGDT()
{
	uint16_t num_gdt_entries = 5;

	//null descriptor, required to be here.
	// -------------------------------------------------------
	gdt_entries[0] = 0;
	// -------------------------------------------------------

	// gdt_entries[1] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    // gdt_entries[2] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    // gdt_entries[3] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    // gdt_entries[4] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));
 
	// -------------------------------------------------------
	uint64_t kernel_code = 0;
	kernel_code |= 0b1011 << 8; // type of selector
	kernel_code |= 1 << 12; // not a system descriptor
	kernel_code |= 0 << 13; // DPL field = 0
	kernel_code |= 1 << 15; // present
	kernel_code |= 1 << 21; // long-mode segment
	gdt_entries[1] = kernel_code << 32;
	// -------------------------------------------------------

	// -------------------------------------------------------
	uint64_t kernel_data = 0;
	kernel_data |= 0b0011 << 8; //type of selector
	kernel_data |= 1 << 12; // not a system descriptor
	kernel_data |= 0 << 13; // DPL field = 0
	kernel_data |= 1 << 15; // present
	kernel_data |= 1 << 21; // long-mode segment
	gdt_entries[2] = kernel_data << 32;
	// -------------------------------------------------------

	// -------------------------------------------------------
	uint64_t user_code = kernel_code | (3 << 13);
	gdt_entries[3] = user_code;
	// -------------------------------------------------------

	// -------------------------------------------------------
	uint64_t user_data = kernel_data | (3 << 13);
	gdt_entries[4] = user_data;
	// // -------------------------------------------------------


	example_gdtr = {
		.limit = uint16_t(num_gdt_entries * sizeof(uint64_t) - 1),
		.base  = (uint64_t)gdt_entries
	};

	__asm__ volatile ("cli");
	__asm__ volatile ("lgdt %0" : : "m"(example_gdtr));
}


