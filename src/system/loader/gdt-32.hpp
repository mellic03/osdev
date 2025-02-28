#pragma once
#include <stddef.h>
#include <stdint.h>

#define IDTSIZE 0xFF
#define GDTSIZE 0xFF
#define IDTBASE 0x00000000
#define GDTBASE 0x00000800

struct ck_gdt_ptr
{
    uint16_t limit;
    uint32_t base;

} __attribute__((packed));


// https://github.com/dreamportdev/Osdev-Notes/blob/master/02_Architecture/04_GDT.md
struct ck_gdt_entry
{
    uint32_t base;
    uint32_t limit;
    uint32_t access;
    uint32_t flags;

} __attribute__((packed));


struct ck_gdt_entry_detailed
{
	uint16_t limit_0_15;    // 16
	uint16_t base_0_15;	    // 16
	uint8_t  base_16_23;	// 8

    uint8_t  access;
	// uint8_t  selector: 	4;
	// uint8_t  is_systype: 1;
	// uint8_t  dpd: 		2;
	// uint8_t  present:	1;

	uint8_t limit_16_19: 4;
	uint8_t available: 	 1;
	uint8_t long_mode: 	 1;
	uint8_t misc:		 1;
	uint8_t granularity: 1;
	uint8_t base_24_32:	 8;

} __attribute__((packed));


struct ck_TSS
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

union ck_gdt_table
{
    ck_gdt_entry entries[16];

    struct {
        ck_gdt_entry null_desc;
        ck_gdt_entry kernel_mode_code;
        ck_gdt_entry kernel_mode_data;
        ck_gdt_entry user_mode_code;
        ck_gdt_entry user_mode_data;
        ck_gdt_entry task_state;
        ck_gdt_entry other[10];
    } __attribute__((packed));

} __attribute__((packed));


// void init_gdt_desc(uint32_t base, uint32_t limite, uint8_t acces, uint8_t other, gdtdesc *desc);
void ckGDT32_create( ck_gdt_table*, ck_TSS* );
void ckGDT32_load( ck_gdt_table*, ck_TSS* );


// extern ck_TSS default_tss;
// extern gdtdesc kgdt[8];
// extern gdtr kgdtr;

