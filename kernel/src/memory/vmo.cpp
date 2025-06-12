// #include "vmo.hpp"
// #include <kernel/memory/pmm.hpp>
// #include <kernel/memory/vmm.hpp>


// struct VmHeader
// {

// };



// void *VmAllocator::alloc( size_t nbytes )
// {
//     // for (auto *obj: m_ranges)
//     // {

//     // }

//     VmAddressRange obj;

//     obj.pbase      = PMM::allocPage();
//     obj.vbase      = VMM::mapPage(obj.pbase, obj.pbase + PMM::hhdm);
//     obj.length     = 0;
//     obj.flags.size = (nbytes < PMM::PAGE_SIZE_2M) ? VmFlag_Page4K : VmFlag_Page2M;
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


