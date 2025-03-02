#include "memory.hpp"
#include "../multiboot2.hpp"

#include <cstdio.hpp>


// const char *MB2TagTypeStr( uint32_t type )
// {
//     switch (type)
//     {
//         default: return "Unknown type";
//         case MULTIBOOT_MEMORY_AVAILABLE: return "MULTIBOOT_MEMORY_AVAILABLE";
//         case MULTIBOOT_MEMORY_RESERVED: return "MULTIBOOT_MEMORY_RESERVED";
//         case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: return "MULTIBOOT_MEMORY_ACPI_RECLAIMABLE";
//         case MULTIBOOT_MEMORY_NVS: return "MULTIBOOT_MEMORY_NVS";
//         case MULTIBOOT_MEMORY_BADRAM: return "MULTIBOOT_MEMORY_BADRAM";
//     }
// }



// int handle_mmap( multiboot_tag *tag, ckMemoryInfo *info )
// {
//     multiboot_memory_map_t *mmap;

//     for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
//     (multiboot_uint8_t *) mmap 
//       < (multiboot_uint8_t *) tag + tag->size;
//     mmap = (multiboot_memory_map_t *) 
//       ((unsigned long) mmap
//        + ((struct multiboot_tag_mmap *) tag)->entry_size))
//     {
//         std::printf("Map 0x%x\n", uint32_t(mmap->addr));

//         if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
//         {
//             uint32_t idx = info->mmap_count;

//             info->nbytes += mmap->len;
//             info->mmap_count += 1;
//             info->mmap_addr[idx] = uint32_t(mmap->addr);
//             info->mmap_size[idx] = uint32_t(mmap->len);
//         }
//         // std::printf(
//         //     "\tbase_addr = 0x%x%x\n",
//         //     unsigned(mmap->addr >> 32),
//         //     unsigned(mmap->addr & 0xffffffff)
//         // );


//         std::printf(
//             "\tbaseptr d: %d \n", int(mmap->addr)
//         );

//         std::printf(
//             "\tbaseptr u: %u \n", uint32_t(mmap->addr)
//         );

//         std::printf(
//             "\tbaseptr x: %x \n", uint32_t(mmap->addr)
//         );

//         // std::printf(
//         //     "\tlength = 0x%x%x\n",
//         //     unsigned(mmap->len >> 32),
//         //     unsigned(mmap->len & 0xffffffff)
//         // );
    
//         std::printf("\n");        
//     }

//     // std::printf("Available: %d\n", count);

//     return 0;
// }




// static ckMemoryInfo ck_memory_info;


// ckMemoryInfo *ckMemoryInit( uint32_t addr )
// {
//     auto *info = &ck_memory_info;

//     info->nbytes = 0;
//     info->mmap_count = 0;

//     std::printf("\n");
//     std::printf("[ckMemoryInit] ------------------------------------------\n");
//     multiboot_tag *tag = (multiboot_tag*)(addr+8);

//     while (tag->type != MULTIBOOT_TAG_TYPE_END)
//     {
//         if (tag->type == MULTIBOOT_TAG_TYPE_MMAP)
//         {
//             handle_mmap(tag, info);
//         }

//         // std::printf(
//         //     "Tag 0x%x, Size 0x%x, Type %s\n",
//         //     tag->type, tag->size, MB2TagTypeStr(tag->type)
//         // );

//         tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
//     }

//     std::printf("---------------------------------------------------------\n");

//     return (addr) ? nullptr : nullptr;
// }
