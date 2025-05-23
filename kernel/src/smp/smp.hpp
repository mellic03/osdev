#pragma once
#include <cpu/cpu.hpp>

struct limine_mp_info;

namespace SMP
{
    void init( void (*entry)(limine_mp_info*) );
    void initSinglecore( void (*entry)(limine_mp_info*) );

    cpu_t     *this_cpu();
    uint64_t   this_cpuid();
    ksched_t  *this_sched();

    kthread_t *this_thread();
    uint64_t   this_tid();
}
