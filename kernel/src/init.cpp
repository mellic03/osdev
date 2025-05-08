#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kernel/boot_limine.hpp>
#include <kernel/memory.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <kernel/log.hpp>

#include "kernel/kvideo.hpp"
#include <kernel/vfs2.hpp>
#include "coreutil/kmalloc.hpp"

#include <string.h>



using  mmap_type = struct { uintptr_t base; size_t size; };
PMM::MemMap pmm_mmaps[32];
size_t      num_mmaps;

extern void LimineRes_init();
extern void kmalloc_init( uintptr_t base, size_t size );
static void load_mmaps();



struct RSDP_t
{
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdtAddress;
} __attribute__ ((packed));



void early_init()
{
    LimineRes_init();

    syslog log("early_init");
    kvideo::initFrontbuffer((uintptr_t)(limine_res.fb));

    for (size_t i=0; i<32; i++)
    {
        pmm_mmaps[i] = {0, 0};
    }

    load_mmaps();
    PMM::init(pmm_mmaps, num_mmaps, limine_res.hhdm);
    VMM::init();
    kmalloc_init(pmm_mmaps[0].base + PMM::hhdm, pmm_mmaps[0].size);
}




static limine_file *find_module( const char *path )
{
    for (size_t i=0; i<limine_res.modules->module_count; i++)
    {
        auto *F = limine_res.modules->modules[i];

        if (strcmp(F->path, path) == 0)
        {
            return F;
        }
    }

    return nullptr;
}


#include <driver/ramfs.hpp>

void late_init()
{
    syslog log("late_init");
    kvideo::initBackbuffer((uintptr_t)(limine_res.fb));
    vfs2::init();

    auto *initrd = find_module("/initrd");

    if (initrd)
    {
        rfs::init(initrd->address);
        // rfs.walk(rfs.m_root->entry_id);
    }


    for (size_t i=0; i<limine_res.modules->module_count; i++)
    {
        auto *F = limine_res.modules->modules[i];

        if (F == initrd)
            continue;
        else
            vfs2::open<vfs2RawFile>(F->path, F->size, F->address);
    }

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

