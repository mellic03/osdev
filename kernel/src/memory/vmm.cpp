#include <kernel/memory/vmm.hpp>
#include <kernel/memory/pmm.hpp>
#include <kassert.h>
#include <cpu/cpu.hpp>

#include <stdio.h>
#include <string.h>
#include <kernel/log.hpp>
#include <kernel/memory.hpp>
#include <kernel/bitmanip.hpp>


static inline void flush_tlb(unsigned long addr)
{
    asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

constexpr uint64_t PML4_INDEX( uint64_t va ) { return (va>>39) & 0x1FF; }
constexpr uint64_t PDP_INDEX ( uint64_t va ) { return (va>>30) & 0x1FF; }
constexpr uint64_t PD_INDEX  ( uint64_t va ) { return (va>>21) & 0x1FF; }
constexpr uint64_t PT_INDEX  ( uint64_t va ) { return (va>>12) & 0x1FF; }


inline static constexpr
uintptr_t PhysToHHDM( uintptr_t addr ) { return addr + PMM::hhdm; }



void VMM_mapPage( uint64_t *pml4, uintptr_t phys, uintptr_t virt, uint64_t flags )
{
    uint64_t *pdp, *pd;

    if (!(pml4[PML4_INDEX(virt)] & VMM::PAGE_PRESENT))
        pml4[PML4_INDEX(virt)] = PMM::allocFrame() | flags;
    pdp = (uint64_t*)PhysToHHDM(pml4[PML4_INDEX(virt)] & ~0xFFF);

    if (!(pdp[PDP_INDEX(virt)] & VMM::PAGE_PRESENT))
        pdp[PDP_INDEX(virt)] = PMM::allocFrame() | flags;
    pd = (uint64_t*)PhysToHHDM(pdp[PDP_INDEX(virt)] & ~0xFFF);


    #ifdef PMM_2MB_PAGES
        pd[PD_INDEX(virt)] = (phys & ~0x1FFFFF) | flags | VMM::PAGE_PAGESIZE;

    #else
        // uint64_t *pt = get_entry(pdt, PT_INDEX(virt), flags);
        // pt[PT_INDEX(virt)] = phys | flags;

    #endif
}

void VMM::mapPage( uintptr_t phys, uintptr_t virt, uint64_t flags )
{
    uint64_t *pml4 = (uint64_t*)PhysToHHDM(CPU::getCR3());
    VMM_mapPage(pml4, phys, virt, flags);
}

void
VMM::mapRange( uintptr_t phys, uintptr_t virt, size_t nbytes, uint64_t flags )
{
    uint64_t *pml4 = (uint64_t*)PhysToHHDM(CPU::getCR3());
    nbytes = idk::align_up(nbytes, PMM::PAGE_SIZE);
    for (size_t offset=0; offset<nbytes; offset+=PMM::PAGE_SIZE)
        VMM_mapPage(pml4, phys+offset, virt+offset, flags);
}




void VMM_unmapPage( uint64_t *pml4, uint64_t virt )
{
    uint64_t *pdp, *pd;

    if (!(pml4[PML4_INDEX(virt)] & VMM::PAGE_PRESENT))
        return;
    pdp = (uint64_t*)PhysToHHDM(pml4[PML4_INDEX(virt)] & ~0xFFF);

    if (!(pdp[PDP_INDEX(virt)] & VMM::PAGE_PRESENT))
        return;
    pd = (uint64_t*)PhysToHHDM(pdp[PDP_INDEX(virt)] & ~0xFFF);

    if (pd[PD_INDEX(virt)] & VMM::PAGE_PAGESIZE) // 2 MiB page
    {
        // PMM::freeBlock(pd[PD_INDEX(virt)] & ~0xFFF);
        pd[PD_INDEX(virt)] = 0; // Clear entry
        flush_tlb(virt);        // Flush TLB
        return;
    }

    kassert(false); // Only 2MiB pages should exist

    // if (!(pd[PD_INDEX(virt)] & VMM::PAGE_PRESENT))
    //     return; // Mapping does not exist
    // uint64_t *pt = (uint64_t *)PhysToHHDM(pd[PD_INDEX(virt)] & ~0xFFF);

    // // Clear the 4 KiB page entry
    // pt[PT_INDEX(virt)] = 0; // Clear the entry
    // flush_tlb(virt);        // Flush TLB
}


void VMM::unmapPage( uintptr_t virt )
{
    uint64_t *pml4 = (uint64_t *)PhysToHHDM(CPU::getCR3());
    VMM_unmapPage(pml4, virt);
}



void VMM::init() 
{
    syslog log("VMM::init");

    uint64_t *pml4 = (uint64_t *)PhysToHHDM(CPU::getCR3());
    log("pml4: 0x%lx", pml4);

    // uint64_t *clone = VMM::clonePML4(pml4);
    // log("clone: 0x%lx", clone);

    // VMM::mapPage(0x0010'0000, 0xDEADBEBE);

    // auto *test = (uint64_t*)0xDEADBEBE;

    // log("writing 123456789 to 0x%lx", test);
    // test[4] = 123456789;
    // log("0x%lx: %u", test, test[4]);

    // VMM::unmapPage(0xDEADBEBE);
}



// uint64_t *VMM::clonePML4( uint64_t *pml4 )
// {
//     uint64_t *pml4_clone = (uint64_t*)PMM::alloc();


//     constexpr auto RE = 512*sizeof(uint64_t);
//     // PMM::PAGE_SIZE;2097152UL

//     for (size_t i=0; i<512; i++)
//     {
//         if (!(pml4[i] & VMM::PAGE_PRESENT))
//             continue;

//         pml4_clone[i] = PMM::alloc() | (pml4[i] & 0xFFF);
//         uint64_t *pdp_clone = (uint64_t*)PhysToHHDM(pml4_clone[i] & ~0xFFF);
//         uint64_t *pdp_src   = (uint64_t*)PhysToHHDM(pml4[i] & ~0xFFF);

//         for (size_t j=0; j<512; j++)
//         {
//             if (!(pdp_src[i] & VMM::PAGE_PRESENT))
//                 continue;

//             pdp_clone[i] = PMM::alloc() | (pdp_src[i] & 0xFFF);
//             // uint64_t *pd_clone = (uint64_t*)PhysToHHDM(pdp_clone[i] & ~0xFFF);
//             // uint64_t *pd_src   = (uint64_t*)PhysToHHDM(pdp_src[i] & ~0xFFF);

//             // memcpy(pd_clone, pd_src, 512*sizeof(uint64_t));
//         }
//     }

//     return pml4_clone;
// }

