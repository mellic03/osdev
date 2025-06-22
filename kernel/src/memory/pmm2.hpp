// #pragma once

// #include <stdint.h>
// #include <stddef.h>
// #include <kernel/memory.hpp>
// #include <util/linear_allocator.hpp>

// #define PMM_2MB_PAGES



// namespace PMM2
// {
//     static constexpr size_t PAGE_SIZE_4K = 4*1024;
//     static constexpr size_t PAGE_SIZE_2M = 2*1024*1024;
//     static constexpr size_t FRAME_SIZE   = 512*sizeof(uint64_t);

//     extern uint64_t hhdm;

//     void init();

//     uintptr_t allocPage4K();
//     uintptr_t allocPage2M();
//     uintptr_t allocFrame();

//     void freePage4K( uintptr_t phys );
//     void freePage2M( uintptr_t phys );
//     void freeFrame( uintptr_t phys );

// }
