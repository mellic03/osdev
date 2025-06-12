#ifndef __is_kernel
    #define __is_kernel
#endif

#include <cpu/cpu.hpp>

#include <driver/pci.hpp>
#include <driver/video.hpp>

#include <kernel/boot_limine.hpp>
#include <kernel/memory.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <kernel/log.hpp>
#include <kernel/module.hpp>
#include "kernel/kmalloc.hpp"
#include <kmemxx.hpp>
#include <kpanic.hpp>

#include <ipc/pipe.hpp>

#include <filesystem/initrd.hpp>
#include <filesystem/ramfs.hpp>
#include <filesystem/vfs.hpp>

#include <string.h>



using mmap_type = struct { uintptr_t base; size_t size; };
PMM::MemMap pmm_mmaps[32];
size_t      num_mmaps;

extern void LimineRes_init();
extern void kmalloc_init( uintptr_t base, size_t size );
static void load_mmaps();


extern "C"
{
    using  ctor_t = void(*)();
    extern ctor_t __init_array_start[];
    extern ctor_t __init_array_end[];

    // extern uint8_t __bss_start[];
    // extern uint8_t __bss_end[];
    extern uint64_t __kernel_end[];
}


void early_init()
{
    LimineRes_init();
    syslog log("early_init");

    for (size_t i=0; i<32; i++)
        pmm_mmaps[i] = {0, 0};
    load_mmaps();

    PMM::init(pmm_mmaps[1], limine_res.hhdm);
    VMM::init();
    kmalloc_init(pmm_mmaps[0].base + PMM::hhdm, pmm_mmaps[0].size);

    {
        // syslog log("call_ctors");
        // int i = 0;
        // log("ctor[%d]: 0x%lx", i++, ctor);

        for (ctor_t *ctor=__init_array_start; ctor<__init_array_end; ctor++)
        {
            (*ctor)();
        }
    }

    PCI::init();

    auto *fb = limine_res.fb->framebuffers[0];
    kvideo2::init();
    kvideo2::setMode(fb->width, fb->height, fb->bpp);

    for (size_t i=0; i<limine_res.modules->module_count; i++)
    {
        auto *file = limine_res.modules->modules[i];
        if (strcmp(file->path, "/initrd.tar.gz") == 0)
            initrd::init(file->address);
    }

    // vfs::init();

}




// static const char *mmap_str( uint32_t type );
static void load_mmaps()
{
    PMM::MemMap *top = pmm_mmaps;

    // Load usable memmaps
    for (uint64_t i=0; i<limine_res.mmaps->entry_count; i++)
    {
        auto *entry = limine_res.mmaps->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            *(top++) = { entry->base, entry->length };
        }
    }

    // Sort by largest
    num_mmaps = top - pmm_mmaps;
    for (size_t i=0; i<num_mmaps; i++)
    {
        for (size_t j=i+1; j<num_mmaps; j++)
        {
            if (pmm_mmaps[i].size < pmm_mmaps[j].size)
            {
                PMM::MemMap tmp = pmm_mmaps[i];
                pmm_mmaps[i] = pmm_mmaps[j];
                pmm_mmaps[j] = tmp;
            }
        }
    }
}





// static const char *mmap_str( uint32_t type )
// {
//     switch (type)
//     {
//         default: return "UNKNOWN_TYPE";
//         case LIMINE_MEMMAP_USABLE:  return "LIMINE_MEMMAP_USABLE";
//         case LIMINE_MEMMAP_RESERVED:    return "LIMINE_MEMMAP_RESERVED";
//         case LIMINE_MEMMAP_ACPI_RECLAIMABLE:    return "LIMINE_MEMMAP_ACPI_RECLAIMABLE";
//         case LIMINE_MEMMAP_ACPI_NVS:    return "LIMINE_MEMMAP_ACPI_NVS";
//         case LIMINE_MEMMAP_BAD_MEMORY:  return "LIMINE_MEMMAP_BAD_MEMORY";
//         case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:  return "LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE";
//         case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:  return "LIMINE_MEMMAP_EXECUTABLE_AND_MODULES";
//         case LIMINE_MEMMAP_FRAMEBUFFER: return "LIMINE_MEMMAP_FRAMEBUFFER";
//     }
// }




// #include <arch/elf.h>

// void clearBSS()
// {
//     auto    *res = limine_res.fh;
//     uint8_t *base = (uint8_t*)(res->executable_file->address);
//     // uint8_t *addrbase = (uint8_t*)(limine_res.execaddr->virtual_base);

//     auto *ehdr  = (Elf64_Ehdr*)base;
//     auto *shdrs = (Elf64_Shdr*)(base + ehdr->e_shoff);

//     // syslog::println("[clearMemory] base:  0x%lx", base);
//     syslog::println("[clearMemory] size: %lu", res->executable_file->size);
//     // syslog::println("[clearMemory] base2: 0x%lx", limine_res.execaddr->virtual_base);
//     // syslog::println("[clearMemory] entry: 0x%lx", ehdr->e_entry);

//     const char *strtab = nullptr;
//     for (uint32_t i=0; i<ehdr->e_shnum; i++)
//     {
//         if (shdrs[i].sh_type == SHT_STRTAB)
//         {
//             strtab = (const char*)(base + shdrs[i].sh_offset);
//         }
//     }

//     // 34'682'368
//     for (uint32_t i=0; i<ehdr->e_shnum; i++)
//     {
//         const auto &shdr = shdrs[i];
//         auto *name = strtab + shdr.sh_name;

//         syslog::println("[clearMemory] name:    %s", name);
//         syslog::println("[clearMemory] addr:    0x%lx", shdr.sh_addr);
//         syslog::println("[clearMemory] offset:  0x%lx", shdr.sh_offset);
//         syslog::println("[clearMemory] size:    %lu",   shdr.sh_size);
//         syslog::println("[clearMemory] entsize: %lu",   shdr.sh_entsize);
//         syslog::println("");

        
//         if (strcmp(name, ".init_array") == 0)
//         {
//             // using ctor_t = void (*)();
//             // Elf64_Addr *funcBeg = (Elf64_Addr*)(base + shdr.sh_offset);
//             // Elf64_Addr *funcEnd = funcBeg + shdr.sh_size/sizeof(void*);

//             // for (Elf64_Addr *addr=funcBeg; addr<funcEnd; addr++)
//             // {
//             //     ((ctor_t)addr)();
//             // }

//             for (size_t j=0; j<shdr.sh_entsize; j++)
//             {
//                 Elf64_Addr sym = ((Elf64_Addr*)(base + shdr.sh_offset))[j];
//                 ((ctor_t)sym)();
//             }
//         }
//     }

// }

