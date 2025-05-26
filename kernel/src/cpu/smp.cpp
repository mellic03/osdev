#include "smp.hpp"
#include <cpu/cpu.hpp>
#include <cpu/scheduler.hpp>
#include <driver/pic.hpp>
#include <kernel/interrupt.hpp>

#include <kernel/boot_limine.hpp>
#include <kernel/log.hpp>
#include <kthread.hpp>
#include <kmalloc.h>
#include <string.h>

#include <stack_vector.hpp>
#include <algorithm>
#include <array>
#include <atomic>
#include <mutex>



// struct ScheduleLock
// {
//     std::atomic_int count{0};

//     void lock()
//     {
//         // CPU::cli();
//         asm volatile ("cli");
//         count++;
//     }

//     void unlock()
//     {
//         if ((--count) == 0)
//             // CPU::sti();
//             asm volatile ("sti");
//     }
// };


static uint64_t bspID = 99999999999;
cpu_t SMP::all_cpus[8];
size_t SMP::num_cpus;


void SMP::init(void (*entry)(limine_mp_info*))
{
    auto *mp = limine_res.mp;
    auto count = mp->cpu_count;
    num_cpus = count;
    bspID = mp->bsp_lapic_id;
    for (size_t i=0; i<count; i++)
    {
        auto *info = mp->cpus[i];
        info->extra_argument = (uint64_t)i;

        if (info->lapic_id != bspID)
            __atomic_store_n(&(info->goto_address), entry, __ATOMIC_RELAXED);
    }

    entry(mp->cpus[bspID]);
}



bool SMP::is_bsp()
{
    return SMP::this_cpu()->id == bspID;
}

uint64_t SMP::bsp_id()
{
    return bspID;
}


cpu_t *SMP::this_cpu()
{
    return (cpu_t*)CPU::rdmsr(CPU::MSR_KERNEL_GS_BASE);
}

uint64_t SMP::this_cpuid()
{
    return SMP::this_cpu()->id;
}

ksched_t *SMP::this_sched()
{
    auto *cpu = SMP::this_cpu();
    return &(cpu->sched);
}

kthread_t *SMP::this_thread()
{
    auto *sched = SMP::this_sched();
    return sched->m_threads.front();
}

uint64_t SMP::this_tid()
{
    return SMP::this_thread()->tid;
}

