// #include "pmm2.hpp"
// #include <kernel/boot_limine.hpp>
// #include <kernel/memory.hpp>
// #include <util/bitmap.hpp>
// #include <stddef.h>

// size_t PMM2::hhdm;

// idk::InplaceBitMap *page_bmap[64];
// size_t              page_nbmaps;


// void PMM2::init()
// {
//     PMM2::hhdm = limine_res.hhdm;

//     auto  *res   = limine_res.mmaps;
//     size_t count = res->entry_count;
//     auto  *mmaps = res->entries;

//     page_nbmaps = 0;

//     for (size_t i=0; i<count; i++)
//     {
//         auto *bmap = (idk::InplaceBitMap*)kmem::bumpAlloc(sizeof(idk::InplaceBitMap));
        
//         *bmap = idk::InplaceBitMap(
//             kmem::bumpAlloc(mmaps[i]->length), mmaps[i]->length
//         );
        
//         page_bmaps[page_nbmaps++] = bmap;
//         // {mmaps[i]->base + PMM2::hhdm, mmaps[i]->length};
//     }

// }





// // uintptr_t PMM2::allocPage4K()
// // {
    
// // }


// // uintptr_t PMM2::allocPage2M()
// // {

// // }


// // uintptr_t PMM2::allocFrame()
// // {

// // }
