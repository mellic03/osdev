#pragma once

#include "./linear_allocator.hpp"
#include "./stack_allocator.hpp"
#include "./buddy_allocator.hpp"
#include "../../bootinfo.hpp"


namespace idk::memory
{
    int init( idk::linear_allocator* );

    idk::buddy_allocator *getBuddy();
}



