#ifndef __is_kernel
    #define __is_kernel
#endif

#include "boot/requests.cpp"
#include "cpu/cpu.hpp"
#include "cpu/smp.hpp"
#include "kvideo/kvideo.hpp"
#include "memory/pmm.hpp"
#include "memory/vmm.hpp"

#include <kmalloc.h>
#include <kernel/buddy_allocator.hpp>
#include <kernel/log.hpp>
#include <kernel/vfs.hpp>

LimineRes reqs;

using  mmap_type = struct { uintptr_t base; size_t size; };
PMM::MemMap pmm_mmaps[32];
size_t      num_mmaps;
bool        processor_enabled[32];
idk::CPU    processor_states[32];


static void load_mmaps();

void early_init()
{
    syslog log("early_init");

    reqs = {
        .hhdm    = lim_hhdm_req.response->offset,
        .fb      = lim_fb_req.response,
        .addr    = lim_addr_req.response,
        .modules = lim_module_req.response,
        .mmaps   = lim_mmap_req.response,
        .mp      = lim_mp_req.response
    };

    for (size_t i=0; i<32; i++)
    {
        pmm_mmaps[i] = { 0, 0 };
        processor_enabled[i] = false;
    }
    processor_states[0].init();

    load_mmaps();
    PMM::init(pmm_mmaps, num_mmaps, reqs.hhdm);
    VMM::init();

    kmalloc_init(pmm_mmaps[0].base + PMM::hhdm, pmm_mmaps[0].size);

}



void system_init()
{
    SMP::init(reqs.mp);
    kvideo::init((uintptr_t)(reqs.fb));

    vfsInsertFile<char>("/dev/kb0/raw",   64, vfsFileFlag_Stream);
    vfsInsertFile<char>("/dev/kb0/event", 64, vfsFileFlag_Stream);
    vfsInsertFile<char>("/dev/ms0/raw",   64, vfsFileFlag_Stream);
    vfsInsertFile<char>("/dev/ms0/event", 64, vfsFileFlag_Stream);
    vfsInsertFile<char>("/dev/stdout",    1024, vfsFileFlag_Stream);

    for (size_t i=0; i<reqs.modules->module_count; i++)
    {
        auto *F = reqs.modules->modules[i];
        vfsInsertFile(F->path, F->address, F->size);
    }

}





// static const char *mmap_str( uint32_t type );

static void load_mmaps()
{
    PMM::MemMap *top = pmm_mmaps;
    
    // Load usable memmaps
    for (uint64_t i=0; i<reqs.mmaps->entry_count; i++)
    {
        auto *entry = reqs.mmaps->entries[i];
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

