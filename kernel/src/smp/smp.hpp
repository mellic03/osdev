#pragma once
#include <cpu/cpu.hpp>

struct limine_mp_info;

namespace SMP
{
    void init( void (*entry)(cpu_t*) );
}
