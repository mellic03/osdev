#pragma once

#include <stdint.h>


// struct PageEntry
// {
//     uint8_t present : 1;
//     uint8_t writable : 1;
//     uint8_t user_accessible : 1;
//     uint8_t write_through_caching : 1;
//     uint8_t disable_cache : 1;
//     uint8_t accessed : 1;
//     uint8_t dirty : 1;
//     uint8_t null : 1;
//     uint8_t global : 1;
//     uint8_t avl1 : 3;
//     uintptr_t physical_address : 50;
//     uint16_t avl2 : 11;
//     uint8_t no_execute : 1;
// };


static constexpr uint64_t MASK_PRESENT    = 1 << 0;
static constexpr uint64_t MASK_WRITE      = 1 << 1;
static constexpr uint64_t MASK_SUPERVISOR = 1 << 2;
static constexpr uint64_t MASK_PWT        = 1 << 3;
static constexpr uint64_t MASK_PCD        = 1 << 4;
static constexpr uint64_t MASK_ACCESSED   = 1 << 5;
static constexpr uint64_t MASK_ADDRESS    = 0x000FFFFFFFFFF000;
static constexpr uint64_t MASK_EXECUTE    = uint64_t(1) << uint64_t(63);

// [48-bit Virtual Address] = [PML4 index][PDPT index][PDT index][PT index][Page Offset]
//                                                                                      000
static constexpr uint64_t ree0 = 0xFFFFFFFFFFFF;
static constexpr uint64_t ree = uint64_t(1) << 48;


union PML4_entry
{
    struct
    {
        uint64_t flags:   6;
        uint64_t other1:  6;  // Just set to zero
        uint64_t address: 38;
        uint64_t other2:  13; // Just set to zero
        uint64_t execute: 1;
    };

    uint64_t qword;

    PML4_entry(): qword(0) {  };
    PML4_entry( uint64_t q ): qword(q) {  };

};

using PageDirectory = uint64_t*;

// struct PageEntry
// {
//     uint64_t p:             1;
//     uint64_t rw:            1;
//     uint64_t us:            1;
//     uint64_t write_through: 1;
//     uint64_t cache_disable: 1;
//     uint64_t accessed:      1;
//     uint64_t avl0:          1;
//     uint64_t resv0:         6;
//     uint64_t addr:          39;
//     uint64_t resv:          1;
//     uint64_t avl:           10;
//     uint64_t xd:            1;
// };



namespace VMM
{
    void init();
}

