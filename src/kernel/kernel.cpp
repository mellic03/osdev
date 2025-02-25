extern "C" {

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#include <kernel/terminal.h>
#include "boot/multiboot.hpp"

#include <libc/stdio.h>
#include <libc/stdio-internal.h>


// /* Check if the compiler thinks you are targeting the wrong operating system. */
// #if defined(__linux__)
// #error "You are not using a cross-compiler, you will most certainly run into trouble"
// #endif

// /* This tutorial will only work for the 32-bit ix86 targets. */
// #if !defined(__i386__)
// #error "This tutorial needs to be compiled with a ix86-elf compiler"
// #endif

struct multiboot_header_t
{
    /* Must be MULTIBOOT_MAGIC - see above. */
    uint32_t magic;

    /* Feature flags. */
    uint32_t flags;

    /* The above fields plus this one must equal 0 mod 2^32. */
    uint32_t checksum;

    /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;

    /* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
    uint32_t mode_type;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
}; // BIGASS_BITCHBOYE __attribute__((section(".multiboot")));


#define KERNEL_START 0xC0000000
#define KERNEL_MALLOC 0xD000000
#define REC_PAGEDIR ((uint32_t*)0xFFFFF000)
#define REC_PAGETABLE(i) ((uint32_t*) (0xFFC00000 + ((i) << 12)))

#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_WRITE (1 << 1)
#define PAGE_FLAG_OWNER (1 << 9)


// uint32_t initial_page_dir[1024];
// int mem_num_vpages;

void initMemory( uint32_t memHigh, uint32_t physicalAllocStart )
{
    // mem_num_vpages = 0;
    // initial_page_dir[0] = 0;
    // invalidate(0);
    // initial_page_dir[1023] = ((uint32_t) initial_page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
    // invalidate(0xFFFFF000);

    // pmm_init(physicalAllocStart, memHigh);
    // memset(pageDirs, 0, 0x1000 * NUM_PAGES_DIRS);
    // memset(pageDirUsed, 0, NUM_PAGES_DIRS);
}




void kernel_main( uint32_t magic, multiboot_info_t *bootinfo ) 
{
	terminal_initialize();


    uint32_t mod1 = *(uint32_t*)(bootinfo->mods_addr + 4);
    uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;

    printf("physicalAllocStart: %d\n", int(physicalAllocStart));
    printf("1024*upper:         %d\n", int(1024 * bootinfo->mem_upper));

	initMemory(bootinfo->mem_upper * 1024, physicalAllocStart);

    // int count = int(bootinfo->mmap_length);
    // auto *mem_maps = (multiboot_memory_map_t *)(bootinfo->mmap_addr);

	// printf("bootinfo->mmap_length; %d\n", int(bootinfo->mmap_length));
    // printf("bootinfo->mmap_addr: %d\n", int(bootinfo->mmap_addr));

    // for (int i=0; i<4; i++)
    // {
    //     auto *mmap = mem_maps + i;

    //     printf("[mmap %d]", i);
    //     printf("  addr=%d", int(mmap->addr));
    //     printf("  len=%d", int(mmap->len));
    //     printf("  size=%d\n", int(mmap->size));
    //     printf("  size=%d\n", int(mmap.));
    // }

	// else if (magic == MULTIBOOT_HEADER_MAGIC)
	// {
	// 	printf("header.magic == MULTIBOOT_HEADER_MAGIC\n");
	// }

	// else if (magic == 3)
	// {
	// 	printf("header.magic == 3\n");
	// }

	// else
	// {
	// 	printf("Rip\n");
	// }



}



}