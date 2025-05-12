#include "smp.hpp"
#include <cpu/cpu.hpp>
#include <cpu/scheduler.hpp>
#include <driver/pic.hpp>
#include <kernel/interrupt.hpp>

#include <kernel/boot_limine.hpp>
#include <kernel/log.hpp>
#include <kthread.hpp>
#include <kmalloc.h>
#include <khang.h>
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
        CPU::cli();
        count++;
    }

    void unlock()
    {
        if ((--count) == 0)
            CPU::sti();
    }
};


static ScheduleLock smp_lock;
// static std::mutex smp_lock{0};
// static cpu_t *bsp_cpu;
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
    
//     kernel::hang();
// }

void SMP::initMulticore(void (*entry)(limine_mp_info*))
{
    syslog log("SMP::initMulticore");
    memset(all_cpus, 0, sizeof(all_cpus));

    auto *mp = limine_res.mp;
    auto count = mp->cpu_count;
    auto bsp_id = mp->bsp_lapic_id;
    limine_mp_info *bsp_info = nullptr;

    // log("cpu_count: %u", count);
    // log("bsp_id:    %u", bsp_id);

    for (size_t i=0; i<count; i++)
    {
        auto *info = mp->cpus[i];
        info->extra_argument = (uint64_t)i;
        // log("cpus[%d].lapic_id: %u", i, info->lapic_id);

        if (info->lapic_id == bsp_id)
        {
            bsp_info = info;
        }
        else
        {
            info->goto_address = entry;
            // __atomic_store_n(&(info->goto_address), &entry, __ATOMIC_RELAXED);
        }
    }

    // log("bsp_info:     0x%lx", bsp_info);
    // log("bsp_info->id: %u", bsp_info->lapic_id);

    entry(bsp_info);
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

