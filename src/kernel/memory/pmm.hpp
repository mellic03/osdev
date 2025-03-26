#pragma once

#include <stdint.h>
#include <stddef.h>
#include <kernel/memory.hpp>

namespace idk
{
    struct Krequests;
}


namespace PMM
{
    static constexpr size_t PAGE_SIZE = 2*idk::MEGA; 
    static constexpr uint64_t PHYS_BASE = 0x100000000;
    extern uint64_t hhdm;

    void init( const idk::Krequests& );
    uintptr_t alloc();

}
