#include "vmm.hpp"
#include "pmm.hpp"
#include <stdio.h>
#include <string.h>
#include "../log/log.hpp"
#include "memory.hpp"
#include <kernel/memory.hpp>
#include <kernel/bitmanip.hpp>


extern "C"
{
    extern uint64_t cpu_get_cr3(void);
    extern void     cpu_set_cr3(uint64_t);
}

static inline void flush_tlb(unsigned long addr) {
    asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}


static constexpr uint64_t MASK_PRESENT    = 1 << 0;
static constexpr uint64_t MASK_WRITE      = 1 << 1;
static constexpr uint64_t MASK_SUPERVISOR = 1 << 2;
static constexpr uint64_t MASK_PWT        = 1 << 3;
static constexpr uint64_t MASK_PCD        = 1 << 4;
static constexpr uint64_t MASK_ACCESSED   = 1 << 5;
static constexpr uint64_t MASK_PAGESIZE   = 1 << 7;
static constexpr uint64_t MASK_ADDRESS    = 0x000FFFFFFFFFF000;
static constexpr uint64_t MASK_EXECUTE    = uint64_t(1) << uint64_t(63);


#define PML4_INDEX(va) (((va) >> 39) & 0x1FF)
#define PDP_INDEX(va) (((va) >> 30) & 0x1FF)
#define PD_INDEX(va) (((va) >> 21) & 0x1FF)
#define PT_INDEX(va) (((va) >> 12) & 0x1FF)

#define KERNEL_VIRT_BASE 0xFFFFFFFF80000000
#define HHDM_BASE 0xFFFF800000000000
#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_HUGE 0x80
// #define PhysToHHDM(addr) (uintptr_t(addr) + HHDM_BASE)

#define PAGE_PRESENT 0x1
#define PAGE_WRITE   0x2
#define PAGE_EXECUTE (uint64_t(1)<<63)  // Optional: Usually 0 since x86 uses NX (bit 63)


inline static constexpr
uintptr_t PhysToHHDM( uintptr_t addr ) { return addr + PMM::hhdm; }



uint64_t *get_entry( uint64_t *table, size_t idx, uint64_t flags )
{
    uint64_t *entry;

    if (!(table[idx] & PAGE_PRESENT))
    {
        uint64_t phys = PMM::alloc();
        entry = (uint64_t *)PhysToHHDM(phys);
        table[idx] = phys | flags;
    }

    else
    {
        entry = (uint64_t *)PhysToHHDM(table[idx] & ~0xFFF);
    }

    return entry;
}

void map_page( uint64_t *pml4, uint64_t phys, uint64_t virt, uint64_t flags )
{
    // Traverse or allocate PDP
    uint64_t *pdp = get_entry(pml4, PML4_INDEX(virt), flags);
    uint64_t *pd  = get_entry(pdp, PDP_INDEX(virt), flags);

    #ifdef PMM_2MB_PAGES
        pd[PD_INDEX(virt)] = (phys & ~0x1FFFFF) | flags | MASK_PAGESIZE;

    #else
        uint64_t *pt = get_entry(pdt, PT_INDEX(virt), flags);
        pt[PT_INDEX(virt)] = phys | flags;

    #endif
}



void
VMM_unmapPage( uint64_t *pml4, uint64_t virt )
{
    // Walk to the PDP
    if (!(pml4[PML4_INDEX(virt)] & PAGE_PRESENT)) {
        return; // Mapping does not exist
    }
    uint64_t *PDP = (uint64_t *)PhysToHHDM(pml4[PML4_INDEX(virt)] & ~0xFFF);

    // Walk to the PDT
    if (!(PDP[PDP_INDEX(virt)] & PAGE_PRESENT)) {
        return; // Mapping does not exist
    }
    uint64_t *pdt = (uint64_t *)PhysToHHDM(PDP[PDP_INDEX(virt)] & ~0xFFF);

    // Check if this is a 2 MiB page
    if (pdt[PD_INDEX(virt)] & 0x80) {
        // This is a 2 MiB page
        pdt[PD_INDEX(virt)] = 0; // Clear the entry
        flush_tlb(virt);         // Flush TLB
        return;
    }

    // Walk to the PT
    if (!(pdt[PD_INDEX(virt)] & PAGE_PRESENT)) {
        return; // Mapping does not exist
    }
    uint64_t *pt = (uint64_t *)PhysToHHDM(pdt[PD_INDEX(virt)] & ~0xFFF);

    // Clear the 4 KiB page entry
    pt[PT_INDEX(virt)] = 0; // Clear the entry
    flush_tlb(virt);        // Flush TLB
}


void
VMM_mapPage( uint64_t *pml4, uintptr_t phys, uintptr_t virt )
{
    map_page(pml4, phys, virt, PAGE_PRESENT|PAGE_WRITE);
}


void
VMM::mapPage( uintptr_t phys, uintptr_t virt )
{
    asm volatile ("cli");
    uint64_t *pml4 = (uint64_t *)PhysToHHDM(cpu_get_cr3());
    VMM_mapPage(pml4, phys, virt);
    asm volatile ("sti");
}


void
VMM::mapRange( uintptr_t phys, uintptr_t virt, size_t nbytes )
{
    asm volatile ("cli");
    uint64_t *pml4 = (uint64_t *)PhysToHHDM(cpu_get_cr3());

    for (size_t offset=0; offset<nbytes; offset+=PMM::PAGE_SIZE)
    {
        VMM_mapPage(pml4, phys+offset, virt+offset);
    }

    asm volatile ("sti");
}


void
VMM::unmapPage( uintptr_t virt )
{
    asm volatile ("cli");
    uint64_t *pml4 = (uint64_t *)PhysToHHDM(cpu_get_cr3());
    VMM_unmapPage(pml4, virt);
    asm volatile ("sti");
}



void VMM::init() 
{
    // syslog log("VMM::init");

    // VMM::mapPage(0x0010'0000, 0xDEADBEBE);

    // auto *test = (uint64_t*)0xDEADBEBE;

    // log("writing 123456789 to 0x%lx", test);
    // test[4] = 123456789;
    // log("0x%lx: %u", test, test[4]);

    // VMM::unmapPage(0xDEADBEBE);
}
