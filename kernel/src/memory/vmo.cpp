#include "vmo.hpp"
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <util/bitmanip.hpp>


struct VmHeader
{

};


// void *VmAllocator::alloc( size_t nbytes )
// {
//     VmAddressRange obj;

//     for (size_t i=0; i<nbytes; i+=PMM::PAGE_SIZE_2M)
//     {
//         uintptr_t phys = PMM::allocPage();
//         VMM::mapPage2(m_pml4, phys, phys, VMM::PAGE_WRITE | VMM::PAGE_PRESENT);
//     }


//     // obj.pbase      = PMM::allocPage();
//     // obj.vbase      = VMM::mapPage(obj.pbase, obj.pbase + PMM::hhdm);
//     // obj.length     = 0;
//     // obj.flags.size = (nbytes < PMM::PAGE_SIZE_2M) ? VmFlag_Page4K : VmFlag_Page2M;
//     // obj.flags.size = VmFlag_Page2M;

//     // if (nbytes < PMM::PAGE_SIZE_4K)
//     // {
//     //     uintptr_t phys = PMM::allocPage();
//     //     obj.size = nbytes;
//     //     // obj.pbase
//     // }

//     return (void*)(obj.vbase);
// }


// void VmAllocator::free( void *ptr )
// {
//     if (ptr)
//     {

//     }

// }



// struct VmPage2M
// {
//     VmHeapNode header;
//     uint8_t data[(2*1024*1024) - sizeof(VmHeapNode)];
// };


// void *VmAllocator::alloc( size_t nbytes )
// {
//     VmHeapNode node = {
//         .nbytes = nbytes
//     };

//     void *ptr;


// }

