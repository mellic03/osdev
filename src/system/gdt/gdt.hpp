#pragma once
#include <stddef.h>
#include <stdint.h>


static constexpr uint16_t GDT_OFFSET_KERNEL_CODE = 0x08;

struct idk_GDTR
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));


struct idk_TSS
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


namespace idk::sys
{
	idk_GDTR loadGDT();
}


