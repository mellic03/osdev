#pragma once

#include <stddef.h>
#include <stdint.h>

// #define KERNEL_START 0xC0000000
// #define KERNEL_MALLOC 0xD000000
// #define REC_PAGEDIR ((unsigned long*)0xFFFFF000)
// #define REC_PAGETABLE(i) ((unsigned long*) (0xFFC00000 + ((i) << 12)))

// #define PAGE_FLAG_PRESENT (1 << 0)
// #define PAGE_FLAG_WRITE (1 << 1)
// #define PAGE_FLAG_OWNER (1 << 9)


// uint32_t initial_page_dir[1024];
// int mem_num_vpages;

#ifdef __cplusplus
extern "C" {
#endif


// int ckInitTerminal();
// int ckInitMemory( unsigned long addr );



#ifdef __cplusplus
}
#endif

// {
//     mem_num_vpages = 0;
//     initial_page_dir[0] = 0;
//     invalidate(0);
//     initial_page_dir[1023] = ((uint32_t) initial_page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
//     invalidate(0xFFFFF000);

//     pmm_init(physicalAllocStart, memHigh);
//     memset(pageDirs, 0, 0x1000 * NUM_PAGES_DIRS);
//     memset(pageDirUsed, 0, NUM_PAGES_DIRS);
// }
