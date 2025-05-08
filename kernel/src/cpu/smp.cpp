#include "smp.hpp"
#include <cpu/cpu.hpp>
#include <cpu/scheduler.hpp>
#include <driver/pic.hpp>
#include <kernel/boot_limine.hpp>
#include <kernel/log.hpp>
#include <kthread.hpp>
#include <kmalloc.h>

#include <stack_vector.hpp>
#include <atomic>
#include <algorithm>


extern void LimineRes_init();

static cpu_t                        *bsp_cpu;
static std::mutex                    smp_mutex;
static std::atomic_bool              smp_ready{false};


constexpr uint32_t MSR_GS_BASE = 0xC0000101; // MSR index for GS.base
static void wrmsr( uint32_t msr, uint64_t value )
{
    asm volatile (
        "wrmsr"
        : /* no outputs */
        : "c"(msr), "a"(value & 0xFFFFFFFF), "d"(value >> 32)
    );
    // asm volatile("wrmsr" : : "c"(msr), "a"(value & 0xFFFFFFFF), "d"(value >> 32));
}



static void smp_init_cpu( limine_mp_info *info )
{
    asm volatile ("cli");

    CPU::enableSSE();
    CPU::installGDT();
    CPU::installIDT();

    cpu_t *cpu = (cpu_t*)(info->extra_argument);
    wrmsr(MSR_GS_BASE, reinterpret_cast<uint64_t>(cpu));

    cpu->createThread("idle", kthread::idlemain, nullptr);
    // CPU::fxsave(cpu->idleThread->fxdata);

    while (smp_ready.load() == false)
        asm volatile ("nop");
    asm volatile ("sti");

    kthread::start();

    while (true) { asm volatile ("hlt"); }
}


// static void smp_init_bsp( cpu_t *cpu )
// {
//     wrmsr(MSR_GS_BASE, reinterpret_cast<uint64_t>(cpu));
// }


cpu_t::cpu_t( uint64_t lapic_id )
:   id(lapic_id)
{
    this->sched      = new ThreadScheduler(this);
    this->idleThread = nullptr;
    this->currThread = nullptr;
}






cpu_t *SMP::init( size_t max_cpus )
{
    asm volatile ("cli");
    smp_ready.store(false);

    auto  *mp     = limine_res.mp;
    auto   count  = std::min(max_cpus, mp->cpu_count);
    auto   bsp_id = mp->bsp_lapic_id;

    for (size_t i=0; i<count; i++)
    {   
        auto *info = mp->cpus[i];
        auto *cpu  = new cpu_t(info->lapic_id);
        cpu->self  = cpu;
        info->extra_argument = (uint64_t)cpu;

        if (cpu->id == bsp_id)
            bsp_cpu = cpu;
        else
            __atomic_store_n(&(info->goto_address), &smp_init_cpu, __ATOMIC_SEQ_CST);
    }

    wrmsr(MSR_GS_BASE, reinterpret_cast<uint64_t>(bsp_cpu));

    return bsp_cpu;
}


void SMP::start()
{
    smp_ready.store(true);
    asm volatile ("sti");
}



cpu_t *SMP::boot_cpu()
{
    return bsp_cpu;
}


cpu_t *SMP::this_cpu()
{
    cpu_t *cpu;
    asm volatile (
        "mov %%gs:0, %0"
        : "=r"(cpu)
    );
    kassert("No cpu", cpu != nullptr);
    return cpu;
}

uint64_t SMP::this_cpuid()
{
    return SMP::this_cpu()->id;
}



ksched_t *SMP::this_sched()
{
    auto *cpu = SMP::this_cpu();
    kassert("No sched", cpu->sched != nullptr);
    return cpu->sched;
}



kthread_t *SMP::this_thread()
{
    auto *cpu = SMP::this_cpu();
    kassert("No thread", cpu->currThread != nullptr);
    return cpu->currThread;
}

uint64_t SMP::this_tid()
{
    return SMP::this_thread()->tid;
}



// #include <string.h>


// static void threadWrapper( void (*fn)(void*), void *arg )
// {
//     fn(arg);
//     auto *th = SMP::this_cpu()->currThread;
    
//     kassert("Bruh", th != nullptr);
//     th->status = Thread_DEAD;

//     while (true) { asm volatile ("hlt"); }
// }



// void SMP::purgatory( limine_mp_info *info, void (*hell)(void*) )
// {
//     asm volatile ("cli");
//     CPU::enableSSE(info->lapic_id);

//     if (info->lapic_id == 1)
//     {
//         hell(nullptr);
//         // gx_main(nullptr);
//     }

//     while (true)
//     {
//         asm volatile ("cli; hlt");
//     }
// }


