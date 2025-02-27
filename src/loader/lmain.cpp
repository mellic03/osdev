#include <kernel/terminal.h>
#include "../common/memory/memory.h"
#include "../common/memory/memory.hpp"
#include "../common/memory/linear_allocator.hpp"
#include "../common/terminal/terminal.h"
#include <libc/stdio.h>

#include "../boot/boot.hpp"
#include "../common/module.hpp"


extern char __multiboot_start;
extern char __multiboot_end;

struct MBH {
    struct multiboot_header header;
    struct multiboot_header_tag_information_request info_req;
    struct multiboot_header_tag_console_flags console_flags;
    struct multiboot_header_tag_framebuffer framebuffer;
    struct multiboot_header_tag end;
};


extern "C" {
    // void lmain();
    void lmain( uint32_t magic, uint32_t addr );
}



static void process_tag( multiboot_tag *tag );
const char *MB2TagTypeStr( unsigned long type );


static int modcount = 0;
static int memcount = 0;


void lmain( uint32_t magic, uint32_t addr ) 
{
    ckTerminal_t term = {
        .VGA_WIDTH  = 80,
        .VGA_HEIGHT = 25,
        .row = 0,
        .col = 0,
        .color = 0,
        .buf = (uint16_t*)(0xB8000)
    };

    ckTerminalInit(&term);


    auto *tag = (multiboot_tag*)(addr+8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END)
    {
        process_tag(tag);
        tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    printf("modcount: %d\n", modcount);

}






static void process_module( multiboot_tag_module *tag )
{
    // printf("Tag: %s\n", MB2TagTypeStr(tag->type));
    modcount += 1;

    printf("\tmod_start: 0x%x\n", tag->mod_start);
    printf("\tmod_end:   0x%x\n", tag->mod_end);
    printf("\tmod_size:  0x%d\n", tag->size);
    printf("\tmod_name:  %s\n",   tag->cmdline);
}

static void process_meminfo( multiboot_tag_basic_meminfo *tag )
{
    printf(
        "mem_lower = 0x%x,  mem+upper = 0x%x\n",
        tag->mem_lower, tag->mem_upper
    );
}

static void process_mmap( multiboot_tag_mmap *tag )
{
    modcount += 1;

    // printf("\tmod_start: 0x%x\n", tag.);
    // printf("\tmod_end:   0x%x\n", tag->mod_end);
    // printf("\tmod_size:  0x%d\n", tag->size);
    // printf("\tmod_name:  %s\n",   tag->cmdline);
}



static void process_tag( multiboot_tag *tag )
{
    printf("Tag: %s\n", MB2TagTypeStr(tag->type));
    ckTerminal_pushIndent(4);

    switch (tag->type)
    {
        default: break;

        case MULTIBOOT_TAG_TYPE_MODULE:
            process_module((multiboot_tag_module*)(tag));
            break;

        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
            process_meminfo((multiboot_tag_basic_meminfo*)(tag));
            break;

        case MULTIBOOT_TAG_TYPE_MMAP:
            process_mmap((multiboot_tag_mmap*)(tag));
            break;
    }

    ckTerminal_popIndent(4);


    // printf("mod_start: 0x%x\n", tag->mod_start);
    // printf("mod_end:   0x%x\n", tag->mod_end);
    // printf("mod_size:  0x%d\n", tag->size);
    // printf("mod_name:  %s\n",   tag->cmdline);

}





const char *MB2TagTypeStr( unsigned long type )
{
    switch (type)
    {
        default: return "UNKNOWN TYPE";
        case MULTIBOOT_TAG_TYPE_END: return "MULTIBOOT_TAG_TYPE_END";
        case MULTIBOOT_TAG_TYPE_CMDLINE: return "MULTIBOOT_TAG_TYPE_CMDLINE";
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: return "MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME";
        case MULTIBOOT_TAG_TYPE_MODULE: return "MULTIBOOT_TAG_TYPE_MODULE";
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: return "MULTIBOOT_TAG_TYPE_BASIC_MEMINFO";
        case MULTIBOOT_TAG_TYPE_BOOTDEV: return "MULTIBOOT_TAG_TYPE_BOOTDEV";
        case MULTIBOOT_TAG_TYPE_MMAP: return "MULTIBOOT_TAG_TYPE_MMAP";
        case MULTIBOOT_TAG_TYPE_VBE: return "MULTIBOOT_TAG_TYPE_VBE";
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: return "MULTIBOOT_TAG_TYPE_FRAMEBUFFER";
        case MULTIBOOT_TAG_TYPE_ELF_SECTIONS: return "MULTIBOOT_TAG_TYPE_ELF_SECTIONS";
        case MULTIBOOT_TAG_TYPE_APM: return "MULTIBOOT_TAG_TYPE_APM";
        case MULTIBOOT_TAG_TYPE_EFI32: return "MULTIBOOT_TAG_TYPE_EFI32";
        case MULTIBOOT_TAG_TYPE_EFI64: return "MULTIBOOT_TAG_TYPE_EFI64";
        case MULTIBOOT_TAG_TYPE_SMBIOS: return "MULTIBOOT_TAG_TYPE_SMBIOS";
        case MULTIBOOT_TAG_TYPE_ACPI_OLD: return "MULTIBOOT_TAG_TYPE_ACPI_OLD";
        case MULTIBOOT_TAG_TYPE_ACPI_NEW: return "MULTIBOOT_TAG_TYPE_ACPI_NEW";
        case MULTIBOOT_TAG_TYPE_NETWORK: return "MULTIBOOT_TAG_TYPE_NETWORK";
        case MULTIBOOT_TAG_TYPE_EFI_MMAP: return "MULTIBOOT_TAG_TYPE_EFI_MMAP";
        case MULTIBOOT_TAG_TYPE_EFI_BS: return "MULTIBOOT_TAG_TYPE_EFI_BS";
        case MULTIBOOT_TAG_TYPE_EFI32_IH: return "MULTIBOOT_TAG_TYPE_EFI32_IH";
        case MULTIBOOT_TAG_TYPE_EFI64_IH: return "MULTIBOOT_TAG_TYPE_EFI64_IH";
        case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR: return "MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR";
    }
}




// void lmain(const void* multiboot_struct) {
//     // Set up GDT
    
//     const multiboot_info_t* mb_info = multiboot_struct;            /* Make pointer to multiboot_info_t struct */
//     multiboot_uint32_t mb_flags = mb_info->flags;                  /* Get flags from mb_info */

//     void* kentry = NULL;                                           /* Pointer to the kernel entry point */

//     if (mb_flags & MULTIBOOT_INFO_MODS)
//     {                          /* Check if modules are available */
//         multiboot_uint32_t mods_count = mb_info->mods_count;   /* Get the amount of modules available */
//         multiboot_uint32_t mods_addr = mb_info->mods_addr;     /* And the starting address of the modules */

//         for (uint32_t mod = 0; mod < mods_count; mod++)
//         {
//             auto *M = (multiboot_module_t*)(mods_addr + (mod * sizeof(multiboot_module_t)));     /* Loop through all modules */
//         }
//     }
// }



// const char* module_string = (const char*)module->cmdline;
// /* Here I check if module_string is equals to the one i assigned my kernel
//    you could skip this check if you had a way of determining the kernel module */
// if(strcmp(module_string, kernel_bin_string)){
//         kentry = load_elf_module(module->mod_start, module->mod_end);
// }


// #include "elf64.h" // Also requires elf64.c

// char* kernel_elf_space[sizeof(elf_file_data_t)];
// elf_file_data_t* kernel_elf = (elf_file_data_t*) kernel_elf_space;                                          /* Pointer to elf file structure (remember there is no memory management yet) */

// /* This function parses the ELF file and returns the entry point */
// void* load_elf_module(multiboot_uint32_t mod_start, multiboot_uint32_t mod_end) {
//         unsigned long err = parse_elf_executable((void*)mod_start, sizeof(elf_file_data_t), kernel_elf);    /* Parses ELF file and returns an error code */
//         if(err == 0){                                                                                       /* No errors occurred while parsing the file */
//                 for(int i = 0; i < kernel_elf->numSegments; i++){
//             elf_file_segment_t seg = kernel_elf->segments[i];                                   /* Load all the program segments into memory */
//                                                                                                 /*  if you want to do relocation you should do so here, */
//             const void* src = (const void*) (mod_start + seg.foffset);                          /*  though that would require some changes to parse_elf_executable */
//             memcpy((void*) seg.address, src, seg.flength);
//         }
//                 return (void*) kernel_elf->entryAddr;                                                       /* Finally we can return the entry address */
//         }
//         return NULL;
// }


// extern void setup_longmode();
// extern void enter_kernel(void* entry, uint32_t multiboot_info);

// void lmain(const void* multiboot_struct) {
//         // Set up GDT
//         // Check modules

//         setup_longmode();                                                                                    /* Set up long mode and jump to the kernel code */
//         enter_kernel(kentry, (uint32_t) mb_info);
// }









