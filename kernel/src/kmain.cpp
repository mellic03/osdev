#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kassert.h>
#include <kmalloc.h>
#include <kpanic.h>
#include <kthread.hpp>
#include <kprintf.hpp>
#include <new>

#include <kernel/boot_limine.hpp>
#include <kernel/clock.hpp>
#include <kernel/event.hpp>
#include <kernel/log.hpp>
#include <sys/interrupt.hpp>
#include <kernel/syscall.h>
#include <kernel/memory/vmm.hpp>
#include <kernel/module.hpp>
#include <kernel/kvideo.hpp>
#include <kprintf.hpp>

#include <wm/wm.hpp>

#include <cpu/cpu.hpp>
#include <cpu/scheduler.hpp>
#include "cpu/smp.hpp"

#include <string.h>
#include <stdio.h>

#include <arch/acpi.hpp>
#include <arch/apic.hpp>
#include <driver/pic.hpp>
#include <driver/pci.hpp>
#include <driver/pit.hpp>
#include <driver/serial.hpp>

#include <filesystem/ramfs.hpp>
#include <filesystem/vfs.hpp>
#include <filesystem/initrd.hpp>

#include "syscall/syscall.hpp"
#include <smp/barrier.hpp>


static void invalidOpISR( intframe_t* );
static void genfaultISR( intframe_t* );
static void pagefaultISR( intframe_t* );
static void outOfMemoryISR( intframe_t* );
static void syscallISR( intframe_t* );
static void spuriousISR( intframe_t* );


static void PIT_IrqHandler( intframe_t *frame )
{
    // syslog::println("[PIT_IRQ] cpu=%lu", SMP::this_cpuid());
    auto *cpu = SMP::this_cpu();
    cpu->m_msecs += 500; // cpu->m_lapicPeriod;
    ThreadScheduler::scheduleISR(frame);
}


extern void LimineRes_init();
extern void early_init();
static void smp_main(limine_mp_info*);
const auto &println = syslog::println;


__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request lim_rsdp_req = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 3
};


static knl::Barrier barrierA{0};
static knl::Barrier barrierB{0};
std::atomic_uint64_t smpCount;
extern uintptr_t endkernel;

static uint64_t  smp_gdt[SMP::max_cpus][5];
static gdt_ptr_t smp_gdtr[SMP::max_cpus];


extern "C"
void _start()
{
    CPU::cli();
    CPU::enableFloat();

    syslog::enable();
    if (!serial::init())
        syslog::disable();
    syslog log("_start");

    CPU::createGDT(smp_gdt[0], &smp_gdtr[0]);
    CPU::installGDT(&smp_gdtr[0]);

    early_init();
    kprintf_init();

    CPU::createIDT();
    PIC::disable();
    PCI::init();

    CPU::installISR(IntNo_InvalidOpcode, invalidOpISR);
    CPU::installISR(IntNo_GenFault,      genfaultISR);
    CPU::installISR(IntNo_PageFault,     pagefaultISR);
    CPU::installISR(IntNo_OUT_OF_MEMORY, outOfMemoryISR);
    CPU::installISR(IntNo_KThreadYield,  ThreadScheduler::scheduleISR);
    CPU::installISR(IntNo_Syscall,       syscallISR);
    CPU::installISR(IntNo_Spurious,      spuriousISR);
    CPU::installIRQ(IrqNo_PIT,           PIT_IrqHandler);

    static ACPI::Response res;
    ACPI::init(lim_rsdp_req.response->address, res);
    APIC::init(res);

    // CPU::featureCheck();
    // VMM::mapPage(0x3000, 0xFFFFFFFFFFF30000);
    // uint64_t *buf = (uint64_t*)0xFFFFFFFFFFF30000;
    // buf[4] = 1234321;
    // log("buf[4]: %lu", buf[4]);
    // VMM::unmapPage(0xFFFFFFFFFFF30000);
    // buf[4] = 8765678;
    // log("buf[4]: %lu", buf[4]);

    smpCount.store(0);
    barrierA.reset(limine_res.mp->cpu_count);
    barrierB.reset(limine_res.mp->cpu_count);
    SMP::init(smp_main);

    kassert(false); // Should be unreachable!

    CPU::hcf();
}




extern ModuleInterface *msdev_init( void* );
extern ModuleInterface *kbdev_init( void* );
extern void load_module2( ModuleInterface* (*)(void*) );


static void smp_main( limine_mp_info *info )
{
    CPU::cli();
    CPU::enableFloat();

    size_t cpuid = info->lapic_id;
    CPU::createGDT(smp_gdt[cpuid], &smp_gdtr[cpuid]);
    CPU::installGDT(&smp_gdtr[cpuid]);
    cpu_t *cpu = new (SMP::get_cpu(cpuid)) cpu_t(cpuid);

    CPU::installIDT();
    LAPIC::init();

    if (cpu->id == SMP::bsp_id())
    {
        // knl::loadModules(initrd::find("drv/"));
        // knl::loadModules(initrd::find("srv/"));
        load_module2(msdev_init);
        load_module2(kbdev_init);
        knl::initModules();
        kthread::create("wm::main", wm::main, nullptr);
        vfs::print();
    }

    // else if (cpu->id == 2)
    // {
    //     vec3 rgb0(0.6, 0.6, 0.85);
    //     vec3 rgb1(0.9, 0.2, 0.65);
    //     vec3 rgb2 = vec_mix(rgb0, rgb1, 0.25f);
    //     u8vec3 rgb = u8vec3(255.0f * rgb2);
    //     syslog::println("rgb3: %u %u %u", rgb.r, rgb.g, rgb.b);
    // }

    barrierB.wait();
    CPU::sti();

    while (true)
    {
        CPU::hlt();
    }
}




void stacktrace( intframe_t *frame )
{
    syslog log("stacktrace");
    intframe_t *sf = frame;

    for (int i=0; i<16; i++)
    {
        log("rip: 0x%lx", sf->rip);
        log("rbp: 0x%lx\n", sf->rbp);
        sf = (intframe_t*)(&sf->rbp);
    }
}

static void invalidOpISR( intframe_t *frame )
{
    syslog log("Exception %u (%s)", frame->isrno, IntNoStr(frame->isrno));

    auto *cpu = SMP::this_cpu();
    auto *th = cpu->sched.currThread();
    if (cpu) log("cpu %d", cpu->id);
    if (th)  log("thread %d (%s)", th, th->name);

}

static void genfaultISR( intframe_t* )
{
    syslog log("Exception GENERAL_PROTECTION_FAULT");

    auto *cpu = SMP::this_cpu();
    auto *th = cpu->sched.currThread();
    if (cpu) log("cpu %d", cpu->id);
    if (th)  log("thread %d (%s)", th, th->name);

    CPU::hcf();
}

static void outOfMemoryISR( intframe_t* )
{
    syslog::print("Exception OUT_OF_MEMORY");
    CPU::hcf();
}

static void syscallISR( intframe_t* )
{
    syslog::print("Exception SYSCALL");
    CPU::hcf();
}

static void spuriousISR( intframe_t* )
{
    return;
}



// static std::atomic_int faultCount{0};

static void pagefaultISR( intframe_t *frame )
{
    syslog log("Exception PAGE_FAULT");

    // if (faultCount++ > 0)
    // {
    //     kpanic("faultCount++ > 0");
    // }

    auto *cpu = SMP::this_cpu();
    auto *th = cpu->sched.currThread();
    log("cpu %d", cpu->id);
    log("thread %d (%s)", th, th->name);

    log("rip:   0x%lx", frame->rip);
    log("rsp:   0x%lx", frame->rsp);
    log("r11:   0x%lx", frame->r11);
    log("r12:   0x%lx", frame->r12);
    log("r13:   0x%lx", frame->r13);
    log("r14:   0x%lx", frame->r14);
    log("r15:   0x%lx", frame->r15);
    log("rax:   0x%lx", frame->rax);
    log("rbx:   0x%lx", frame->rbx);
    log("rcx:   0x%lx", frame->rcx);
    log("rdx:   0x%lx", frame->rdx);
    log("rdi:   0x%lx", frame->rdi);
    log("rsi:   0x%lx", frame->rsi);
    log("rbp:   0x%lx", frame->rbp);
    log("isrno: %lu", frame->isrno);
    log("errno: %lu", frame->errno);

    uint64_t flags = frame->errno;
    // printBits(flags);

    if (flags & (1<<0))
         log("\t- Page-protection violation");
    else log("\t- Non-present page");

    if (flags & (1<<1))
         log("\t- Caused by write-access");
    else log("\t- Caused by read-access");

    if (flags & (1<<2))
        log("\t- Caused while CPL=3, not necessarily privilege violation");

    if (flags & (1<<3))
        log("\t- One or more page directory entries contain reserved bits which are set to 1. This only applies when the PSE or PAE flags in CR4 are set to 1. ");

    if (flags & (1<<4))
        log("\t- Caused by instruction fetch");

    if (flags & (1<<5))
        log("\t- Caused by a protection-key violation");

    if (flags & (1<<6))
        log("\t- Caused by a shadow stack access");

    if (flags & (1<<15))
        log("\t- Fault was due to an SGX violation. The fault is unrelated to ordinary paging.");
    
    kpanic("Page fault");
}






