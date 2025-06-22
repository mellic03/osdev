#include "smp.hpp"
#include <smp/smp.hpp>
#include <smp/barrier.hpp>
#include <cpu/cpu.hpp>

#include <arch/acpi.hpp>
#include <arch/apic.hpp>

#include <driver/pic.hpp>
#include <sys/interrupt.hpp>

#include <kernel/boot_limine.hpp>
#include <kernel/log.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <kthread.hpp>
#include <kmalloc.h>
#include <string.h>

#include <stack_vector.hpp>
#include <algorithm>
#include <array>
#include <atomic>
#include <mutex>
#include <new>

size_t SMP::num_cpus;
static uint64_t bspID = 0;
static cpu_t    SMP_cpus[SMP::max_cpus];

static void (*SMP_kmain)(cpu_t*);
static uint64_t  smp_gdt[SMP::max_cpus][5];
static gdt_ptr_t smp_gdtr[SMP::max_cpus];

void SMP_initwrapper( limine_mp_info *info )
{
    CPU::cli();

    size_t cpuid = info->lapic_id;
    CPU::createGDT(smp_gdt[cpuid], &smp_gdtr[cpuid]);
    CPU::installGDT(&smp_gdtr[cpuid]);
    CPU::enableFloat();

    cpu_t *cpu = new (SMP::get_cpu(cpuid)) cpu_t(cpuid);
    CPU::wrmsr(CPU::MSR_GS_BASE, (uint64_t)cpu);
    CPU::wrmsr(CPU::MSR_GS_KERNEL_BASE, (uint64_t)cpu);

    CPU::installIDT();
    LAPIC::init();

    SMP_kmain(cpu);
}


void SMP::init( void (*entry)(cpu_t*) )
{
    SMP_kmain = entry;

    auto *mp = limine_res.mp;
    SMP::num_cpus = mp->cpu_count;
    bspID = mp->bsp_lapic_id;

    for (size_t i=0; i<SMP::num_cpus; i++)
    {
        auto *info = mp->cpus[i];
        info->extra_argument = (uint64_t)i;

        if (info->lapic_id != bspID)
            __atomic_store_n(&(info->goto_address), &SMP_initwrapper, __ATOMIC_RELAXED);
    }

    SMP_initwrapper(mp->cpus[bspID]);
}



bool SMP::is_bsp()
{
    return SMP::this_cpu()->id == bspID;
}

uint64_t SMP::bsp_id()
{
    return bspID;
}

cpu_t *SMP::get_cpu( uint32_t idx )
{
    return &SMP_cpus[idx];
}

cpu_t *SMP::this_cpu()
{
    return (cpu_t*)CPU::rdmsr(CPU::MSR_GS_BASE);
}

uint64_t SMP::this_cpuid()
{
    return SMP::this_cpu()->id;
}

knl::Sched *SMP::this_sched()
{
    auto *cpu = SMP::this_cpu();
    return &cpu->sched2;
}

knl::Thread *SMP::this_thread()
{
    auto *sd = SMP::this_sched();
    return sd->m_threads.front();
}


