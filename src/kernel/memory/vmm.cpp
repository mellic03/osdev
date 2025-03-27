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


union PD_Entry
{
    struct
    {
        uint64_t present:   1;  // 0
        uint64_t rw:        1;  // 1
        uint64_t us:        1;  // 2
        uint64_t pwt:       1;  // 3
        uint64_t pcd:       1;  // 4
        uint64_t A:         1;  // 5
        uint64_t res0:      1;  // 6
        uint64_t PS:        1;  // 7  -If set, entry points to a 2MB page.
                                //     If clear, entry points to a Page Table (PT).
                                //
                                //
        uint64_t res1:      1;  // 8
        uint64_t avl0:      3;  // 9 10 11
        uint64_t address:   28; // Table base address
                                // 12 13 14 15 16 17 18 19 20 21
                                // 22 23 24 25 26 27 28 29 30 31
                                // 32 33 34 35 36 37 38 39
                                //
        uint64_t res2:      12; // 40 41 42 43 44 45 46 47 48 49
                                // 50 51
                                //
        uint64_t avl1:      7;  // 52 53 54 55 56 57 58
        uint64_t pk:        3;  // 59 60 61 62
        uint64_t xd:        1;  // 63
    };
    
    uint64_t qword;

} __attribute__((packed));


union PT_Entry
{
    struct
    {
        uint64_t present:   1;  // 0
        uint64_t rw:        1;  // 1
        uint64_t us:        1;  // 2
        uint64_t pwt:       1;  // 3
        uint64_t pcd:       1;  // 4
        uint64_t A:         1;  // 5
        uint64_t D:         1;  // 6
        uint64_t pat:       1;  // 7
        uint64_t G:         1;  // 8
        uint64_t avl0:      3;  // 9 10 11
        uint64_t address:   28; // Page base address
                                // 12 13 14 15 16 17 18 19 20 21
                                // 22 23 24 25 26 27 28 29 30 31
                                // 32 33 34 35 36 37 38 39
                                //
        uint64_t res2:      12; // 40 41 42 43 44 45 46 47 48 49
                                // 50 51
                                //
        uint64_t avl1:      7;  // 52 53 54 55 56 57 58
        uint64_t pk:        3;  // 59 60 61 62
        uint64_t xd:        1;  // 63
    };
    
    uint64_t qword;

} __attribute__((packed));


#define PML4_INDEX(va) (((va) >> 39) & 0x1FF)
#define PDPT_INDEX(va) (((va) >> 30) & 0x1FF)
#define PD_INDEX(va) (((va) >> 21) & 0x1FF)
#define PT_INDEX(va) (((va) >> 12) & 0x1FF)

#define KERNEL_VIRT_BASE 0xFFFFFFFF80000000
#define HHDM_BASE 0xFFFF800000000000
#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_HUGE 0x80
#define PHYS_TO_HHDM(addr) (uintptr_t(addr) + HHDM_BASE)

#define PAGE_PRESENT 0x1
#define PAGE_WRITE   0x2
#define PAGE_EXECUTE (uint64_t(1)<<63)  // Optional: Usually 0 since x86 uses NX (bit 63)



void map4KB(uint64_t *pml4, uint64_t phys, uint64_t virt, uint64_t flags)
{
    // Traverse or allocate PDPT
    uint64_t *pdpt;
    if (!(pml4[PML4_INDEX(virt)] & PAGE_PRESENT)) {
        uint64_t pdpt_phys = PMM::alloc();
        pdpt = (uint64_t *)PHYS_TO_HHDM(pdpt_phys);
        pml4[PML4_INDEX(virt)] = pdpt_phys | flags;
    } else {
        pdpt = (uint64_t *)PHYS_TO_HHDM(pml4[PML4_INDEX(virt)] & ~0xFFF);
    }

    // Traverse or allocate PDT
    uint64_t *pdt;
    if (!(pdpt[PDPT_INDEX(virt)] & PAGE_PRESENT)) {
        uint64_t pdt_phys = PMM::alloc();
        pdt = (uint64_t *)PHYS_TO_HHDM(pdt_phys);
        pdpt[PDPT_INDEX(virt)] = pdt_phys | flags;
    } else {
        pdt = (uint64_t *)PHYS_TO_HHDM(pdpt[PDPT_INDEX(virt)] & ~0xFFF);
    }

    // Traverse or allocate PT
    uint64_t *pt;
    if (!(pdt[PT_INDEX(virt)] & PAGE_PRESENT)) {
        uint64_t pt_phys = PMM::alloc();
        pt = (uint64_t *)PHYS_TO_HHDM(pt_phys);
        pdt[PT_INDEX(virt)] = pt_phys | flags;
    } else {
        pt = (uint64_t *)PHYS_TO_HHDM(pdt[PT_INDEX(virt)] & ~0xFFF);
    }

    // Map the page
    pt[PT_INDEX(virt)] = phys | flags;
}


void map2MB( uint64_t *pml4, uint64_t phys, uint64_t virt, uint64_t flags )
{
    // Traverse or allocate PDPT
    uint64_t *pdpt;
    if (!(pml4[PML4_INDEX(virt)] & PAGE_PRESENT)) {
        uint64_t pdpt_phys = PMM::alloc();
        pdpt = (uint64_t *)PHYS_TO_HHDM(pdpt_phys);
        pml4[PML4_INDEX(virt)] = pdpt_phys | flags;
    } else {
        pdpt = (uint64_t *)PHYS_TO_HHDM(pml4[PML4_INDEX(virt)] & ~0xFFF);
    }

    // Traverse or allocate PDT
    uint64_t *pdt;
    if (!(pdpt[PDPT_INDEX(virt)] & PAGE_PRESENT)) {
        uint64_t pdt_phys = PMM::alloc();
        pdt = (uint64_t *)PHYS_TO_HHDM(pdt_phys);
        pdpt[PDPT_INDEX(virt)] = pdt_phys | flags;
    } else {
        pdt = (uint64_t *)PHYS_TO_HHDM(pdpt[PDPT_INDEX(virt)] & ~0xFFF);
    }

    pdt[PD_INDEX(virt)] = (phys & ~0x1FFFFF) | flags | MASK_PAGESIZE;
}


void
VMM_unmapPage( uint64_t *pml4, uint64_t virt )
{
    // Walk to the PDPT
    if (!(pml4[PML4_INDEX(virt)] & PAGE_PRESENT)) {
        return; // Mapping does not exist
    }
    uint64_t *pdpt = (uint64_t *)PHYS_TO_HHDM(pml4[PML4_INDEX(virt)] & ~0xFFF);

    // Walk to the PDT
    if (!(pdpt[PDPT_INDEX(virt)] & PAGE_PRESENT)) {
        return; // Mapping does not exist
    }
    uint64_t *pdt = (uint64_t *)PHYS_TO_HHDM(pdpt[PDPT_INDEX(virt)] & ~0xFFF);

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
    uint64_t *pt = (uint64_t *)PHYS_TO_HHDM(pdt[PD_INDEX(virt)] & ~0xFFF);

    // Clear the 4 KiB page entry
    pt[PT_INDEX(virt)] = 0; // Clear the entry
    flush_tlb(virt);        // Flush TLB
}


void
VMM_mapPage( uint64_t *pml4, uintptr_t phys, uintptr_t virt )
{
    #ifdef PMM_2MB_PAGES
        map2MB(pml4, phys, virt, PAGE_PRESENT|PAGE_WRITE);
    #else
        map4KB(pml4, phys, virt, PAGE_PRESENT|PAGE_WRITE);
    #endif
}


void
VMM::mapPage( uintptr_t phys, uintptr_t virt )
{
    asm volatile ("cli");
    uint64_t *pml4 = (uint64_t *)PHYS_TO_HHDM(cpu_get_cr3());
    VMM_mapPage(pml4, phys, virt);
    asm volatile ("sti");
}


void
VMM::mapRange( uintptr_t phys, uintptr_t virt, size_t nbytes )
{
    asm volatile ("cli");
    uint64_t *pml4 = (uint64_t *)PHYS_TO_HHDM(cpu_get_cr3());

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
    uint64_t *pml4 = (uint64_t *)PHYS_TO_HHDM(cpu_get_cr3());
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
