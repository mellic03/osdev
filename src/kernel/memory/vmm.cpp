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
    extern uint64_t __cpu_get_cr3(void);
    extern void     __cpu_set_cr3(uint64_t);
}

const uint64_t &hhdm = PMM::hhdm;


#define MMU_RECURSIVE_SLOT      (510UL)


// Convert an address into array index of a structure
// E.G. int index = MMU_PML4_INDEX(0xFFFFFFFFFFFFFFFF); // index = 511
#define MMU_PML4_INDEX(addr_)    ((((uintptr_t)(addr_))>>39) & 511)
#define MMU_PDPT_INDEX(addr_)    ((((uintptr_t)(addr_))>>30) & 511)
#define MMU_PD_INDEX(addr_)      ((((uintptr_t)(addr_))>>21) & 511)
#define MMU_PT_INDEX(addr_)      ((((uintptr_t)(addr_))>>12) & 511)

// Base address for paging structures
#define KADDR_MMU_PT            (0xFFFF000000000000UL + (MMU_RECURSIVE_SLOT<<39))
#define KADDR_MMU_PD            (KADDR_MMU_PT         + (MMU_RECURSIVE_SLOT<<30))
#define KADDR_MMU_PDPT          (KADDR_MMU_PD         + (MMU_RECURSIVE_SLOT<<21))
#define KADDR_MMU_PML4          (KADDR_MMU_PDPT       + (MMU_RECURSIVE_SLOT<<12))

// Structures for given address, for example
// uint64_t* pt = MMU_PT(addr)
// uint64_t physical_addr = pt[MMU_PT_INDEX(addr)];
#define MMU_PML4(addr_)          ((uint64_t*)  KADDR_MMU_PML4 )
#define MMU_PDPT(addr_)          ((uint64_t*)( KADDR_MMU_PDPT + (((addr_)>>27) & 0x00001FF000) ))
#define MMU_PD(addr_)            ((uint64_t*)( KADDR_MMU_PD   + (((addr_)>>18) & 0x003FFFF000) ))
#define MMU_PT(addr_)            ((uint64_t*)( KADDR_MMU_PT   + (((addr_)>>9)  & 0x7FFFFFF000) ))






int indent;
bool found;
uint64_t lvls[4];
uint64_t result[4];
PML4_entry PML4;
PageDirectory rootdir;


void walk_page_tables( uintptr_t phys, int level )
{
    #define INDENT { for (int k=0; k<indent; k++) printf(" ", k); }

    if (found == false)
    {
        lvls[level-1] = phys;
    }

    if (level == 1)
    {
        INDENT
        printf("[level 1] 0x%lx --> 0x%lx\n", phys, phys+PMM::hhdm);

        if (phys == 0x100000000)
        {
            INDENT
            printf("Found!\n");
            memcpy(result, lvls, 4*sizeof(uint64_t));
            found = true;
        }
        return;
    }

    else
    {
        INDENT
        printf("[level %d] [phys 0x%lx]\n", level, phys);
    }


    INDENT
    printf("{\n");
    indent += 4;

    for (int i=0; i<512; i++)
    {
        uint64_t *addr = (uint64_t*)(PMM::hhdm + phys + i*sizeof(uint64_t));
        uint64_t entry = *addr;

        if ((entry & MASK_PRESENT) == 0)
        {
            continue;
        }

        uint64_t next = entry & MASK_ADDRESS;
        walk_page_tables(next, level-1);
    }

    indent -= 4;
    INDENT
    printf("}\n");

}


static inline void vmm_flush_tlb(uintptr_t page ) 
{
	__asm__ volatile ("invlpg (%0)" :: "r" (page) : "memory");
}



PageDirectory vmm_get_page_map_level( PageDirectory pmlx, uintptr_t idx, int flags)
{
    if (pmlx[idx] & 1)
        return (PageDirectory)((pmlx+hhdm/sizeof(uint64_t))[idx] & ~(511));
    else
    {
        pmlx[idx] = (PMM::alloc() - hhdm) | flags;
        return (PageDirectory)((pmlx+hhdm/sizeof(uint64_t))[idx] & ~(511));
    }
}


void map_page( PageDirectory curr_pml4, uintptr_t phys, uintptr_t virt, uint32_t flags )
{
    syslog log("map_page");

    uintptr_t index4 = (virt & ((uintptr_t)0x1ff << 39)) >> 39;
    uintptr_t index3 = (virt & ((uintptr_t)0x1ff << 30)) >> 30;
    uintptr_t index2 = (virt & ((uintptr_t)0x1ff << 21)) >> 21;
    uintptr_t index1 = (virt & ((uintptr_t)0x1ff << 12)) >> 12;

    PageDirectory pml4 = curr_pml4;
    log("pml4: 0x%lx", pml4);

    PageDirectory pml3 = vmm_get_page_map_level(pml4, index4, flags);
    log("pml3: 0x%lx", pml3);

    PageDirectory pml2 = vmm_get_page_map_level(pml3, index3, flags);
    log("pml2: 0x%lx", pml2);

    PageDirectory pml1 = vmm_get_page_map_level(pml2, index2, flags);
    log("pml1: 0x%lx", pml1);


    (pml1+hhdm/sizeof(uint64_t))[index1] = phys | flags;
    log("pml1[index1]: 0x%lx", phys | flags);

    vmm_flush_tlb(virt);
}

void unmap_page( PageDirectory curr_pml4, uintptr_t virt )
{
    syslog log("unmap_page");

    uintptr_t index4 = (virt & ((uintptr_t)0x1ff << 39)) >> 39;
    uintptr_t index3 = (virt & ((uintptr_t)0x1ff << 30)) >> 30;
    uintptr_t index2 = (virt & ((uintptr_t)0x1ff << 21)) >> 21;
    uintptr_t index1 = (virt & ((uintptr_t)0x1ff << 12)) >> 12;

    PageDirectory pml4 = curr_pml4;
    PageDirectory pml3 = NULL;
    PageDirectory pml2 = NULL;
    PageDirectory pml1 = NULL;

    pml3 = vmm_get_page_map_level(pml4, index4, 0);
    pml2 = vmm_get_page_map_level(pml3, index3, 0);
    pml1 = vmm_get_page_map_level(pml2, index2, 0);

    pml1[index1] = 0;

    vmm_flush_tlb(virt);
}

void VMM::init() 
{
    // syslog log("VMM::init");

    // uint64_t cr3 = __cpu_get_cr3();

    // // rootdir = (PageDirectory)cr3;
    // rootdir = (PageDirectory)(PMM::alloc() - hhdm);
    // memset((uint8_t*)rootdir + hhdm, 0, PMM::PAGE_SIZE);

	// // PML4 = __cpu_get_cr3();
    // // PML4 = MASK_PRESENT | MASK_WRITE | MASK_SUPERVISOR;
    // // PML4.address = PMM::alloc();
    // log("PML4 qword:   0x%lx", PML4.qword);
    // log("PML4 address: 0x%lx", PML4.address);

    // map_page(
    //     rootdir,
    //     (uintptr_t)0x00000000AFF26000,
    //     (uintptr_t)0xFFFFFFFF800A0000,
    //     MASK_PRESENT|MASK_WRITE|MASK_SUPERVISOR
    // );

    // __cpu_set_cr3((uint64_t)rootdir);
    
}
