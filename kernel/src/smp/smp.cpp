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



// struct KSemaphore
// {
//     std::atomic_int value{1};

//     void wait()
//     {
//         value--;
//         while (value.load() < 0)
//             asm volatile ("nop");
//     }

//     void signal()
//     {
//         value++;
//     }

// };


struct ScheduleLock
{
    std::atomic_int count{0};

    void lock()
    {
        // CPU::cli();
        asm volatile ("cli");
        count++;
    }

    void unlock()
    {
        if ((--count) == 0)
            // CPU::sti();
            asm volatile ("sti");
    }
};


static ScheduleLock smp_lock;
// static std::mutex smp_lock{0};
static uint64_t bsp_id = 99999999999;
cpu_t SMP::all_cpus[8];

// cpu_t::cpu_t()
//     : self(this),
//       id(lapic_id),
//       sched(*this),
//       yldrsn(YldRsn_None)
// {
//     this->idleThread = nullptr; // sched.addThread("idlemain", kthread::idlemain, nullptr);
//     this->currThread = nullptr; // this->idleThread;
// }

// static void SMP_trampoline( limine_mp_info *info )
// {
//     cpu_t *cpu = (cpu_t*)(info->extra_argument);
//     CPU::wrmsr(CPU::MSR_GS_BASE, reinterpret_cast<uint64_t>(cpu));
//     kthread::create("idlemain", kthread::idlemain, nullptr);
//     // syslog::println("[SMP_trampoline] cpu=%lu", SMP::this_cpu()->id);
//     CPU::enableSSE();
//     CPU::installGDT();
    
//     knl::hang();
// }

void SMP::initMulticore(void (*entry)(limine_mp_info*))
{
    syslog log("SMP::initMulticore");
    memset(all_cpus, 0, sizeof(all_cpus));

    auto *mp = limine_res.mp;
    auto count = mp->cpu_count;
    bsp_id = mp->bsp_lapic_id;
    limine_mp_info *bsp_info = nullptr;

    for (size_t i=0; i<count; i++)
    {
        auto *info = mp->cpus[i];
        info->extra_argument = (uint64_t)i;

        if (info->lapic_id == bsp_id)
            bsp_info = info;
        else
            __atomic_store_n(&(info->goto_address), entry, __ATOMIC_RELAXED);
    }

    entry(bsp_info);
}


void SMP::initSinglecore(void (*entry)(limine_mp_info*))
{
    syslog log("SMP::initMulticore");
    memset(all_cpus, 0, sizeof(all_cpus));

    auto *mp = limine_res.mp;
    auto count = mp->cpu_count;
    bsp_id = mp->bsp_lapic_id;
    limine_mp_info *bsp_info = nullptr;

    for (size_t i=0; i<count; i++)
    {
        auto *info = mp->cpus[i];
        info->extra_argument = (uint64_t)i;

        if (info->lapic_id == bsp_id)
            bsp_info = info;
    }

    entry(bsp_info);
}


bool SMP::is_bsp()
{
    return SMP::this_cpu()->id == bsp_id;
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

