#include <cpu/tss.hpp>
#include <string.h>
#include <kmalloc.h>
#include <kernel/memory/pmm.hpp>
// #include <Paging.h>
// #include <PhysicalAllocator.h>

extern "C"
{
    void LoadTSS(uint64_t address, uint64_t gdt, uint8_t selector);
}

namespace TSS
{
    void InitializeTSS( tss_t *tss, void *gdt )
    {
        LoadTSS((uintptr_t)tss, (uint64_t)gdt, 0x28);
        memset(tss, 0, sizeof(tss_t));

        // Set up Interrupt Stack Tables
        tss->ist1 = (uint64_t)kmalloc(8*4096); // ::KernelAllocate4KPages(8);
        tss->ist2 = (uint64_t)kmalloc(8*4096); // ::KernelAllocate4KPages(8);
        tss->ist3 = (uint64_t)kmalloc(8*4096); // ::KernelAllocate4KPages(8);

        memset((void*)tss->ist1, 0, 4096);
        memset((void*)tss->ist2, 0, 4096);
        memset((void*)tss->ist3, 0, 4096);

        tss->ist1 += 8*4096;
        tss->ist2 += 8*4096;
        tss->ist3 += 8*4096;

        asm volatile("mov %%rsp, %0" : "=r"(tss->rsp0));
        asm volatile("ltr %%ax" ::"a"(0x2B));
    }
}