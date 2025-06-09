#include <kernel/memory/vmm.hpp>
#include <kernel/memory/pmm.hpp>
#include <kassert.h>
#include <cpu/cpu.hpp>

#include <stdio.h>
#include <string.h>
#include <kernel/log.hpp>
#include <kernel/memory.hpp>
#include <kernel/bitmanip.hpp>


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
    phys = idk::align_down(phys, PMM::PAGE_SIZE);
    virt = idk::align_down(virt, PMM::PAGE_SIZE);
    uint64_t *pml4 = (uint64_t*)PhysToHHDM(CPU::getCR3());
    VMM_mapPage(pml4, phys, virt, flags);
}

void VMM::mapPage2( uintptr_t pml4_phys, uintptr_t phys, uintptr_t virt,
                    uint64_t flags )
{
    phys = idk::align_down(phys, PMM::PAGE_SIZE);
    virt = idk::align_down(virt, PMM::PAGE_SIZE);
    uint64_t *pml4_virt = (uint64_t*)PhysToHHDM(pml4_phys);
    VMM_mapPage(pml4_virt, phys, virt, flags);
}


void
VMM::mapRange( uintptr_t phys, uintptr_t virt, size_t npages, uint64_t flags )
{
    phys = idk::align_down(phys, PMM::PAGE_SIZE);
    virt = idk::align_down(virt, PMM::PAGE_SIZE);
    uint64_t *pml4 = (uint64_t*)PhysToHHDM(CPU::getCR3());

    for (size_t i=0; i<npages; i++)
    {
        size_t offset = i*PMM::PAGE_SIZE;
        VMM_mapPage(pml4, phys+offset, virt+offset, flags);
    }
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

    if (pd[PD_INDEX(virt)] == 0)    
        return;

    if (pd[PD_INDEX(virt)] & VMM::PAGE_PAGESIZE) // 2 MiB page
    {
        pd[PD_INDEX(virt)] = 0; // Clear entry
        CPU::flushTLB(virt);    // Flush TLB
        return;
    }

    kassert(false); // Only 2MiB pages should exist

    if (!(pd[PD_INDEX(virt)] & VMM::PAGE_PRESENT))
        return; // Mapping does not exist
    uint64_t *pt = (uint64_t*)PhysToHHDM(pd[PD_INDEX(virt)] & ~0xFFF);

    // Clear the 4 KiB page entry
    pt[PT_INDEX(virt)] = 0; // Clear the entry
    CPU::flushTLB(virt);    // Flush TLB
}


void VMM::unmapPage( uintptr_t virt )
{
    virt = idk::align_down(virt, PMM::PAGE_SIZE);
    uint64_t *pml4 = (uint64_t *)PhysToHHDM(CPU::getCR3());
    VMM_unmapPage(pml4, virt);
    CPU::flushTLB(virt);
}


void VMM::unmapRange( uintptr_t virt, size_t npages )
{
    virt = idk::align_down(virt, PMM::PAGE_SIZE);
    uint64_t *pml4 = (uint64_t *)PhysToHHDM(CPU::getCR3());
    
    for (size_t i=0; i<npages; i++)
        VMM_unmapPage(pml4, virt + i*PMM::PAGE_SIZE);

    CPU::flushTLB(virt);
}



void VMM::init() 
{
    syslog log("VMM::init");
    uint64_t *pml4 = (uint64_t*)PhysToHHDM(CPU::getCR3());
    log("pml4: 0x%lx", pml4);
}


void *VMM::alloc( size_t nbytes, uint64_t flags )
{
    size_t npages = (nbytes+PMM::PAGE_SIZE) / PMM::PAGE_SIZE;
    uintptr_t phys = PMM::allocPages(npages);
    uintptr_t virt = phys + PMM::hhdm;
    VMM::mapRange(phys, virt, npages, flags);

    return (void*)virt;
}


void VMM::free( void *virt, size_t nbytes )
{
    size_t npages = (nbytes+PMM::PAGE_SIZE) / PMM::PAGE_SIZE;
    VMM::unmapRange((uintptr_t)virt, npages);
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

