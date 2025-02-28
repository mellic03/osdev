#include <kcommon/memory.hpp>
#include <kcommon/terminal.hpp>
#include <kcommon/module.hpp>
#include <kcommon/multiboot2.hpp>
#include <kcommon/serial.hpp>

#include <kcommon/stack.hpp>


#include <stdc++/cstdio.hpp>
#include <stdc++/cstring.hpp>

#include "./gdt-32.hpp"

// extern char __multiboot_start;
// extern char __multiboot_end;

struct MBH {
    struct multiboot_header header;
    struct multiboot_header_tag_information_request info_req;
    struct multiboot_header_tag_console_flags console_flags;
    struct multiboot_header_tag_framebuffer framebuffer;
    struct multiboot_header_tag end;
};


extern "C" {
    void lmain( uint32_t magic, uint32_t addr );
    void _enter_kernel( void *entry, uint32_t multiboot_info );
}

static ckMemoryInfo *ckMemoryInit( uint32_t addr );
static void process_tag( multiboot_tag *tag );
const char *MB2TagTypeStr( unsigned long type );


static int modcount = 0;


// void testfn( int depth )
// {
//     auto sp = ckStackPointer();
//     std::printf("[testfn %d] stack pointer: 0x%x\n", depth, sp);

//     if (depth < 4)
//     {
//         testfn(depth+1);
//     }

//     return;
// }


void lmain( uint32_t magic, uint32_t addr ) 
{
    uint16_t vga_buffer[25*80];

    ckTerminal term(80, 25, &(vga_buffer[0]));
    ckTerminalInit(term);

    std::printf("sizeof(void*) == %d\n", sizeof(void*));
    term.flush();

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        return;
    }

    std::printf("magic == MULTIBOOT2_BOOTLOADER_MAGIC == 0x%x\n", magic);
    term.flush();

    if (ck::serial::init() == 1)
    {
        std::printf("Failed to initialize serial\n");
        return;
    }

    std::printf("Initialized serial\n");
    term.flush();
    ck::serial::writemsg("It is working, my dudes.\n\0\0\0");

    ck_gdt_table GDT;
    ck_TSS TSS;
    ckGDT32_create(&GDT, &TSS);
    ckGDT32_load(&GDT, &TSS);
    std::printf("[lmain] gdt initalized\n");
    term.flush();

    auto *tag = (multiboot_tag*)(addr+8);
    while (tag->type != MULTIBOOT_TAG_TYPE_END)
    {
        process_tag(tag);
        term.flush();
        tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
    }


    auto *info = ckMemoryInit(addr);
    if (info == (void*)(0xDEADBEEF))
    {
        return;
    }

    for (int i=0; i<10; i++)
    {
        std::printf("wwerwerwe\n");
        term.flush();
    }


}






static void process_module( multiboot_tag_module *tag )
{
    std::printf("Tag: %s\n", MB2TagTypeStr(tag->type));
    modcount += 1;

    std::printf("\tmod_start: 0x%x\n", tag->mod_start);
    std::printf("\tmod_end:   0x%x\n", tag->mod_end);
    std::printf("\tmod_size:  0x%d\n", tag->size);
    std::printf("\tmod_name:  %s\n",   tag->cmdline);
}


// static void process_meminfo( multiboot_tag_basic_meminfo *tag )
// {
//     std::printf(
//         "mem_lower = 0x%x,  mem+upper = 0x%x\n",
//         tag->mem_lower, tag->mem_upper
//     );
// }



static void process_tag( multiboot_tag *tag )
{
    std::printf("Tag: %s\n", MB2TagTypeStr(tag->type));
    // ckTerminal_pushIndent(4);

    switch (tag->type)
    {
        default: break;

        case MULTIBOOT_TAG_TYPE_MODULE:
            process_module((multiboot_tag_module*)(tag));
            break;

        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
            // process_meminfo((multiboot_tag_basic_meminfo*)(tag));
            break;

        // case MULTIBOOT_TAG_TYPE_MMAP:
        //     process_mmap((multiboot_tag_mmap*)(tag));
        //     break;
    }

    // ckTerminal_popIndent(4);


    // std::printf("mod_start: 0x%x\n", tag->mod_start);
    // std::printf("mod_end:   0x%x\n", tag->mod_end);
    // std::printf("mod_size:  0x%d\n", tag->size);
    // std::printf("mod_name:  %s\n",   tag->cmdline);

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
        std::printf("Map 0x%x\n", uint32_t(mmap->addr));

        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            uint32_t idx = info->mmap_count;

            info->nbytes += mmap->len;
            info->mmap_count += 1;
            info->mmap_addr[idx] = uint32_t(mmap->addr);
            info->mmap_size[idx] = uint32_t(mmap->len);
        }
        // std::printf(
        //     "\tbase_addr = 0x%x%x\n",
        //     unsigned(mmap->addr >> 32),
        //     unsigned(mmap->addr & 0xffffffff)
        // );


        std::printf(
            "\tbaseptr d: %d \n", int(mmap->addr)
        );

        std::printf(
            "\tbaseptr u: %u \n", uint32_t(mmap->addr)
        );

        std::printf(
            "\tbaseptr x: %x \n", uint32_t(mmap->addr)
        );

        // std::printf(
        //     "\tlength = 0x%x%x\n",
        //     unsigned(mmap->len >> 32),
        //     unsigned(mmap->len & 0xffffffff)
        // );
    
        std::printf("\n");        
    }

    // std::printf("Available: %d\n", count);

    return 0;
}



static ckMemoryInfo ck_memory_info;

ckMemoryInfo *ckMemoryInit( uint32_t addr )
{
    auto *info = &ck_memory_info;

    info->nbytes = 0;
    info->mmap_count = 0;

    std::printf("\n");
    std::printf("[ckMemoryInit] ------------------------------------------\n");
    multiboot_tag *tag = (multiboot_tag*)(addr+8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END)
    {
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP)
        {
            handle_mmap(tag, info);
        }

        // std::printf(
        //     "Tag 0x%x, Size 0x%x, Type %s\n",
        //     tag->type, tag->size, MB2TagTypeStr(tag->type)
        // );

        tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    std::printf("---------------------------------------------------------\n");

    return (addr) ? nullptr : nullptr;
}

