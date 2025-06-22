#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kassert.h>
#include <kpanic.hpp>
#include <kthread.hpp>
#include <kprintf.hpp>
#include <new>

#include <kernel/boot_limine.hpp>
#include <kernel/clock.hpp>
#include <kernel/event.hpp>
#include <kernel/log.hpp>
#include <sys/interrupt.hpp>
#include <kernel/syscall.h>
#include <kernel/memory.hpp>
#include <kernel/memory/vmm.hpp>
#include <kernel/module.hpp>
#include <kernel/kvideo.hpp>

#include <wm/wm.hpp>

#include <cpu/cpu.hpp>
#include "smp/smp.hpp"
#include <sys/process.hpp>

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

extern ModuleInterface *msdev_init( void* );
extern ModuleInterface *kbdev_init( void* );
extern void load_module2( ModuleInterface* (*)(void*) );

static knl::Barrier barrierA{0};



static void kmain( cpu_t *cpu )
{
    if (cpu->id == SMP::bsp_id())
    {
        // knl::loadModules(initrd::find("drv/"));
        // knl::loadModules(initrd::find("srv/"));
        load_module2(msdev_init);
        load_module2(kbdev_init);
        knl::initModules();
        // vfs::print();

        knl::createProcess("wm::main", wm::main, nullptr);
    }

    barrierA.wait();
    CPU::sti();

    while (true)
    {
        CPU::hlt();
    }
}






#include <driver/svga.hpp>
#include <driver/video.hpp>
#include <util/linkedlist.hpp>


static void pagefaultISR( intframe_t* );
static void outOfMemoryISR( intframe_t* );
static void syscallISR( intframe_t* );
static void spuriousISR( intframe_t* );

static void PIT_IrqHandler( intframe_t *frame )
{
    // syslog::println("[PIT_IRQ] cpu=%lu", SMP::this_cpuid());
    auto *cpu = SMP::this_cpu();
    cpu->m_ticks += 500;
    knl::Sched::scheduleISR(frame);
}



extern void knl_ClearBSS();
extern void knl_ClearMemory();
extern void early_init();
extern void CPU_featureCheck();
extern void CPU_featureCheck2();
const auto &println = syslog::println;


extern "C"
void _start()
{
    CPU::cli();
    // knl_ClearBSS();
    // knl_ClearMemory();

    CPU::createGDT();
    CPU::installGDT();
    CPU::initFoat();

    (serial::init()) ? syslog::enable() : syslog::disable();
    early_init();
    CPU_featureCheck2();

    PIC::disable();
    CPU::createIDT();
    CPU::installIDT();

    CPU::installISR(IntNo_PageFault,     pagefaultISR);
    CPU::installISR(IntNo_OUT_OF_MEMORY, outOfMemoryISR);
    CPU::installISR(IntNo_KThreadYield,  knl::Sched::scheduleISR);
    CPU::installISR(IntNo_Syscall,       syscallISR);
    CPU::installISR(IntNo_Spurious,      spuriousISR);
    CPU::installIRQ(IrqNo_PIT,           PIT_IrqHandler);

    static ACPI::Response res;
    ACPI::init(limine_res.rsdp->address, res);
    APIC::init(res);

    // ustar::listAll(initrd::tarball);

    // void *handle = knl::dl_open("usr/lib/libc.a");
    // syslog::println("handle: 0x%lx", handle);
    // void *addr1  = knl::sym_load(handle, "ulibc_add");
    // syslog::println("addr1:  0x%lx", addr1);

    // auto testfn = (int (*)(int, int))addr1;
    // syslog::println("YOLO\n");
    // int value = testfn(2, 98);
    // syslog::println("res: %d\n", value);

    barrierA.reset(limine_res.mp->cpu_count);
    SMP::init(kmain);

    kassert(false); // Should be unreachable!

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



static void pagefaultISR( intframe_t *frame )
{
    syslog log("Exception PAGE_FAULT");

    auto *cpu = SMP::this_cpu();
    log("cpu %d", cpu->id);

    auto *th = cpu->sched2.currThread();
    if (th && th->process)
    {
        log("process: %s", th->process->m_name);
    }


    log("rax:    0x%lx", frame->rax);
    log("rbx:    0x%lx", frame->rbx);
    log("rcx:    0x%lx", frame->rcx);
    log("rdx:    0x%lx", frame->rdx);
    log("rdi:    0x%lx", frame->rdi);
    log("rsi:    0x%lx", frame->rsi);
    log("rbp:    0x%lx", frame->rbp);
    log("isrno:  %lu",   frame->isrno);
    log("errno:  %lu",   frame->errno);
    log("rip:    0x%lx", frame->rip);
    log("cs:     0x%lx", frame->cs);
    log("rflags: 0x%lx", frame->rflags);
    log("rsp:    0x%lx", frame->rsp);
    log("ss:     0x%lx", frame->ss);

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



