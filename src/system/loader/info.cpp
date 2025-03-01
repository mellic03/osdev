#include "info.hpp"
#include <kcommon/multiboot2.hpp>
#include "../drivers/serial.hpp"

#include <stdc++/cstdio.hpp>
#include <stdc++/cstring.hpp>


static ckInfo __ck_info;

const char *MB2TagTypeStr( unsigned long type );
const char *MB2MMapTypeStr( uint32_t type );

void __ckProcessTag( multiboot_tag* );
void __ckProcessTag( multiboot_tag_string* );
void __ckProcessTag( multiboot_tag_module* );
void __ckProcessTag( multiboot_tag_basic_meminfo* );
void __ckProcessTag( multiboot_tag_bootdev* );
void __ckProcessTag( multiboot_tag_mmap* );
void __ckProcessTag( multiboot_vbe_info_block* );
void __ckProcessTag( multiboot_vbe_mode_info_block* );
void __ckProcessTag( multiboot_tag_vbe* );
void __ckProcessTag( multiboot_tag_framebuffer* );
void __ckProcessTag( multiboot_tag_elf_sections* );
void __ckProcessTag( multiboot_tag_apm* );
void __ckProcessTag( multiboot_tag_efi32* );
void __ckProcessTag( multiboot_tag_efi64* );
void __ckProcessTag( multiboot_tag_smbios* );
void __ckProcessTag( multiboot_tag_old_acpi* );
void __ckProcessTag( multiboot_tag_new_acpi* );
void __ckProcessTag( multiboot_tag_network* );
void __ckProcessTag( multiboot_tag_efi_mmap* );
void __ckProcessTag( multiboot_tag_efi32_ih* );
void __ckProcessTag( multiboot_tag_efi64_ih* );
void __ckProcessTag( multiboot_tag_load_base_addr* );



ckInfo
ckInfoLoad( uint32_t *addr )
{
    __ck_info.memory.num_mmaps  = 0;
    __ck_info.memory.total_size = 0;

    // __ck_info.modules.kernel_x64 = {nullptr, nullptr};
    __ck_info.font = 0; // = {nullptr, nullptr};

	// ck::serial::writestr("[ckInfoLoad]\n");

    auto *tag = (multiboot_tag*)(addr);

    while (tag->type != MULTIBOOT_TAG_TYPE_END)
    {
        __ckProcessTag(tag);
        tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    return __ck_info;
}



void __ckProcessTag( multiboot_tag *tag )
{
	// ck::serial::writestr("[ckProcessTag]\n");
    std::printf("Tag type: %s\n", MB2TagTypeStr(tag->type));

    switch (tag->type)
    {
        default: break;

        case MULTIBOOT_TAG_TYPE_MODULE:
            __ckProcessTag((multiboot_tag_module*)tag);
            break;

        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
            __ckProcessTag((multiboot_tag_basic_meminfo*)tag);
            break;

        case MULTIBOOT_TAG_TYPE_MMAP:
            __ckProcessTag((multiboot_tag_mmap*)tag);
            break;

        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
            __ckProcessTag((multiboot_tag_framebuffer*)tag);
            break;
    }
}





void __ckProcessTag( multiboot_tag_module *tag )
{
    // auto &mods = __ck_info.modules;

    std::printf("\tname:   %s\n",   tag->cmdline);
    std::printf("\tstart:  0x%x\n", tag->mod_start);
    std::printf("\tend:    0x%x\n", tag->mod_end);
    std::printf("\tnbytes: %u\n", tag->mod_end - tag->mod_start);

    if (std::strcmp(tag->cmdline, "kernel") == 0)
    {
        // mods.kernel_x64 = {(uint32_t*)(tag->mod_start), (uint32_t*)(tag->mod_end)};
    }

    else if (std::strcmp(tag->cmdline, "font-test") == 0)
    {
        // std::printf("size: 0x%u\n", tag->mod_end - tag->mod_start);
        __ck_info.font = tag->mod_start;
        // mods.font = {(uint32_t*)(tag->mod_start), (uint32_t*)(tag->mod_end)};
    }
}



void __ckProcessTag( multiboot_tag_basic_meminfo *tag )
{
    std::printf(
        "\tmem_lower: 0x%x\n\tmem+upper: 0x%x\nmem_size: 0x%x\n",
        tag->mem_lower, tag->mem_upper, tag->size
    );
}

void __ckProcessTag( multiboot_tag_mmap *tag )
{
    multiboot_memory_map_t *mmap;

    for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
    (multiboot_uint8_t *) mmap 
      < (multiboot_uint8_t *) tag + tag->size;
    mmap = (multiboot_memory_map_t *) 
      ((unsigned long) mmap
       + ((struct multiboot_tag_mmap *) tag)->entry_size))
    {
        // std::printf("\tMap 0x%x %s\n", uint32_t(mmap->addr), MB2MMapTypeStr(mmap->type));
        // std::printf("\t\taddress: 0x%x\n", mmap->addr);
        // std::printf("\t\tlength:  %u bytes\n", mmap->len);

        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            // auto idx = __ck_info.memory.num_mmaps++;

            // __ck_info.memory.total_size += mmap->len;

            // __ck_info.memory.mmaps[idx] = ckMemblock(
            //     mmap->addr,
            //     mmap->addr + mmap->len
            // );

            std::printf("\t0x%x", uint32_t(mmap->addr >> 32));
            std::printf("%x -- ", uint32_t(mmap->addr));
            std::printf("%u -- ", (mmap->len));
            std::printf("AVAILABLE\n");
        }
    }

}


void __ckProcessTag( multiboot_tag_framebuffer *tag )
{
    std::printf("\taddress: 0x%x\n", tag->common.framebuffer_addr);

    std::printf(
        "\twidth: %d\n\theight: %d\n",
        tag->common.framebuffer_width, tag->common.framebuffer_height
    );
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



const char *MB2MMapTypeStr( uint32_t type )
{
    switch (type)
    {
        default: return "Unknown type";
        case MULTIBOOT_MEMORY_AVAILABLE: return "AVAILABLE";
        case MULTIBOOT_MEMORY_RESERVED: return "RESERVED";
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: return "ACPI_RECLAIMABLE";
        case MULTIBOOT_MEMORY_NVS: return "NVS";
        case MULTIBOOT_MEMORY_BADRAM: return "BADRAM";
    }
}




