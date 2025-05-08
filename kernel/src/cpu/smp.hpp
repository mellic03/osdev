#pragma once
#include <cpu/cpu.hpp>

namespace SMP
{
    cpu_t *init( size_t max_cpus=0 );
    void start();
    
}



