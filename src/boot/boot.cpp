#include "boot.hpp"
#include <libc/stdio.h>

// const char *MB2TagTypeStr( unsigned long );


// int
// ck::kboot( unsigned long magic, unsigned long addr )
// {
//     if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
//     {
//         printf("[kernel::kboot] Invalid magic number: 0x%u\n", int(magic));
//         return -1;
//     }

//     if (addr & 7)
//     {
//         printf ("[kernel::kboot] Unaligned mbi: 0x%x\n", addr);
//         return -1;
//     }

//     unsigned long size = *(unsigned long*)addr;
//     printf("[kernel::kboot] Announced mbi size 0x%x\n", size);


//     multiboot_tag *tag = (multiboot_tag*)(addr+8);

//     while (tag->type != MULTIBOOT_TAG_TYPE_END)
//     {
//         printf(
//             "Tag 0x%x, Size 0x%x, Type %s\n",
//             tag->type, tag->size, MB2TagTypeStr(tag->type)
//         );

//         tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
//     }

//     return 0;
// }





// const char *MB2TagTypeStr( unsigned long type )
// {
//     switch (type)
//     {
//         default: return "UNKNOWN TYPE";
//         case MULTIBOOT_TAG_TYPE_END: return "MULTIBOOT_TAG_TYPE_END";
//         case MULTIBOOT_TAG_TYPE_CMDLINE: return "MULTIBOOT_TAG_TYPE_CMDLINE";
//         case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: return "MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME";
//         case MULTIBOOT_TAG_TYPE_MODULE: return "MULTIBOOT_TAG_TYPE_MODULE";
//         case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: return "MULTIBOOT_TAG_TYPE_BASIC_MEMINFO";
//         case MULTIBOOT_TAG_TYPE_BOOTDEV: return "MULTIBOOT_TAG_TYPE_BOOTDEV";
//         case MULTIBOOT_TAG_TYPE_MMAP: return "MULTIBOOT_TAG_TYPE_MMAP";
//         case MULTIBOOT_TAG_TYPE_VBE: return "MULTIBOOT_TAG_TYPE_VBE";
//         case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: return "MULTIBOOT_TAG_TYPE_FRAMEBUFFER";
//         case MULTIBOOT_TAG_TYPE_ELF_SECTIONS: return "MULTIBOOT_TAG_TYPE_ELF_SECTIONS";
//         case MULTIBOOT_TAG_TYPE_APM: return "MULTIBOOT_TAG_TYPE_APM";
//         case MULTIBOOT_TAG_TYPE_EFI32: return "MULTIBOOT_TAG_TYPE_EFI32";
//         case MULTIBOOT_TAG_TYPE_EFI64: return "MULTIBOOT_TAG_TYPE_EFI64";
//         case MULTIBOOT_TAG_TYPE_SMBIOS: return "MULTIBOOT_TAG_TYPE_SMBIOS";
//         case MULTIBOOT_TAG_TYPE_ACPI_OLD: return "MULTIBOOT_TAG_TYPE_ACPI_OLD";
//         case MULTIBOOT_TAG_TYPE_ACPI_NEW: return "MULTIBOOT_TAG_TYPE_ACPI_NEW";
//         case MULTIBOOT_TAG_TYPE_NETWORK: return "MULTIBOOT_TAG_TYPE_NETWORK";
//         case MULTIBOOT_TAG_TYPE_EFI_MMAP: return "MULTIBOOT_TAG_TYPE_EFI_MMAP";
//         case MULTIBOOT_TAG_TYPE_EFI_BS: return "MULTIBOOT_TAG_TYPE_EFI_BS";
//         case MULTIBOOT_TAG_TYPE_EFI32_IH: return "MULTIBOOT_TAG_TYPE_EFI32_IH";
//         case MULTIBOOT_TAG_TYPE_EFI64_IH: return "MULTIBOOT_TAG_TYPE_EFI64_IH";
//         case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR: return "MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR";
//     }
// }
