#include "memory.h"
#include "memory.hpp"
#include "linear_allocator.hpp"
#include "../boot/boot.hpp"
#include <libc/stdio.h>


#define KERNEL_START 0xC0000000
#define KERNEL_MALLOC 0xD000000
#define REC_PAGEDIR ((unsigned long*)0xFFFFF000)
#define REC_PAGETABLE(i) ((unsigned long*) (0xFFC00000 + ((i) << 12)))

#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_WRITE (1 << 1)
#define PAGE_FLAG_OWNER (1 << 9)


unsigned long initial_page_dir[1024];
int mem_num_vpages;




const char *MB_MMapTypeStr( uint32_t type )
{
    switch (type)
    {
        default: return "Unknown type";
        case MULTIBOOT_MEMORY_AVAILABLE: return "MULTIBOOT_MEMORY_AVAILABLE";
        case MULTIBOOT_MEMORY_RESERVED: return "MULTIBOOT_MEMORY_RESERVED";
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: return "MULTIBOOT_MEMORY_ACPI_RECLAIMABLE";
        case MULTIBOOT_MEMORY_NVS: return "MULTIBOOT_MEMORY_NVS";
        case MULTIBOOT_MEMORY_BADRAM: return "MULTIBOOT_MEMORY_BADRAM";
    }
}



int handle_mmap( multiboot_tag *tag, ckMemoryInfo *info )
{
    multiboot_memory_map_t *mmap;

    for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
    (multiboot_uint8_t *) mmap 
      < (multiboot_uint8_t *) tag + tag->size;
    mmap = (multiboot_memory_map_t *) 
      ((unsigned long) mmap
       + ((struct multiboot_tag_mmap *) tag)->entry_size))
    {
        printf("Map 0x%x\n", uint32_t(mmap->addr));

        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            uint32_t idx = info->mmap_count;

            info->mmap_count += 1;
            info->mmap_addr[idx] = uint32_t(mmap->addr);
            info->mmap_size[idx] = uint32_t(mmap->len);
        }
        // printf(
        //     "\tbase_addr = 0x%x%x\n",
        //     unsigned(mmap->addr >> 32),
        //     unsigned(mmap->addr & 0xffffffff)
        // );


        printf(
            "\tbaseptr d: %d \n", int(mmap->addr)
        );

        printf(
            "\tbaseptr u: %u \n", uint32_t(mmap->addr)
        );

        printf(
            "\tbaseptr x: %x \n", uint32_t(mmap->addr)
        );

        // printf(
        //     "\tlength = 0x%x%x\n",
        //     unsigned(mmap->len >> 32),
        //     unsigned(mmap->len & 0xffffffff)
        // );
    
        printf("\n");        
    }

    // printf("Available: %d\n", count);

    return 0;
}




int ckMemoryInit( unsigned long addr, ckMemoryInfo *info )
{
    info->mmap_count = 0;

    printf("\n");
    printf("[ckMemoryInit] ------------------------------------------\n");
    multiboot_tag *tag = (multiboot_tag*)(addr+8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END)
    {
        // if (tag->type == MULTIBOOT_TAG_TYPE_MMAP)
        // {
        //     handle_mmap(tag, info);
        // }

        if (tag->type == MULTIBOOT_TAG_TYPE_MODULE)
        {
            printf("Found MULTIBOOT_TAG_TYPE_MODULE\n");
        }

        // printf(
        //     "Tag 0x%x, Size 0x%x, Type %s\n",
        //     tag->type, tag->size, MB2TagTypeStr(tag->type)
        // );

        tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    // mem_num_vpages = 0;
    // initial_page_dir[0] = 0;
    // invalidate(0);
    // initial_page_dir[1023] = ((unsigned long) initial_page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
    // invalidate(0xFFFFF000);

    // pmm_init(physicalAllocStart, memHigh);
    // memset(pageDirs, 0, 0x1000 * NUM_PAGES_DIRS);
    // memset(pageDirUsed, 0, NUM_PAGES_DIRS);
    printf("---------------------------------------------------------\n");

    return 0;
}
