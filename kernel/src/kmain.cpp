#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kassert.h>
#include <kmalloc.h>
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
#include <kernel/memory/vmm.hpp>
#include <kernel/module.hpp>
#include <kernel/kvideo.hpp>
#include <kprintf.hpp>

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

#include <fixed.hpp>


extern ModuleInterface *msdev_init( void* );
extern ModuleInterface *kbdev_init( void* );
extern void load_module2( ModuleInterface* (*)(void*) );

static knl::Barrier barrierA{0};
static knl::Barrier barrierB{0};


static void kmain( cpu_t *cpu )
{
    if (cpu->id == SMP::bsp_id())
    {
        // Fixed f = 0.5f;
        // uint32_t whole = f.value / Fixed::Scale;
        // uint32_t fract = 0; // (f.value >> Fixed::Shift);
        // uint32_t divisor = 2;
        // for (int i=15; i>=0; i--)
        // {
        //     (f.value & (1<<i)) ? syslog::print("1") : syslog::print("0");

        //     if (f.value & (1<<i))
        //         fract += Fixed::Scale*divisor;
        //     divisor *= 2;
        // } 
        // syslog::println("");
        // syslog::println("[kmain] value: %u.%u", whole, fract);

        // knl::loadModules(initrd::find("drv/"));
        // knl::loadModules(initrd::find("srv/"));
        load_module2(msdev_init);
        load_module2(kbdev_init);
        knl::initModules();
        vfs::print();

        // kthread::create("wm::main", wm::main, nullptr);
        knl::createProcess("wm::main", wm::main, nullptr);
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









#include <driver/svga.hpp>
#include <driver/video.hpp>
#include <kernel/linkedlist.hpp>


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


__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request lim_rsdp_req = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 3
};


// static void smp_hell( limine_mp_info* )
// {
//     CPU::cli();
//     CPU::hcf();
// }

// #include <arch/elf.h>

// void knl_reboot()
// {
//     cpu_t *cpu = SMP::this_cpu();
//     auto  *res = limine_res.mp;

//     for (size_t i=0; i<SMP::num_cpus; i++)
//     {
//         if (SMP::get_cpu(i)->id != cpu->id)
//         {
//             __atomic_store_n(
//                 &(res->cpus[i]->goto_address), &smp_hell, __ATOMIC_SEQ_CST
//             );
//         }
//     }

//     auto *ehdr = (Elf64_Ehdr*)(limine_res.fh->executable_file->address);
//     auto entryfn = (void (*)())(ehdr->e_entry);
//     syslog::println("[knl_reboot] entry: 0x%lx", entryfn);
//     entryfn();
// }


extern "C"
void _start()
{
    CPU::cli();
    // knl_ClearBSS();
    // knl_ClearMemory();
    CPU_featureCheck();
    CPU::enableFloat();
    
    CPU::createGDT();
    CPU::installGDT();
    
    (serial::init()) ? syslog::enable() : syslog::disable();
    early_init();

    CPU_featureCheck2();

    CPU::createIDT();
    PIC::disable();

    // CPU::installISR(IntNo_InvalidOpcode, invalidOpISR);
    // CPU::installISR(IntNo_GenFault,      genfaultISR);
    CPU::installISR(IntNo_PageFault,     pagefaultISR);
    CPU::installISR(IntNo_OUT_OF_MEMORY, outOfMemoryISR);
    CPU::installISR(IntNo_KThreadYield,  knl::Sched::scheduleISR);
    CPU::installISR(IntNo_Syscall,       syscallISR);
    CPU::installISR(IntNo_Spurious,      spuriousISR);
    CPU::installIRQ(IrqNo_PIT,           PIT_IrqHandler);

    static ACPI::Response res;
    ACPI::init(lim_rsdp_req.response->address, res);
    APIC::init(res);

    // smpCount.store(0);
    barrierA.reset(limine_res.mp->cpu_count);
    barrierB.reset(limine_res.mp->cpu_count);
    SMP::init(kmain);

    kassert(false); // Should be unreachable!

    CPU::hcf();
}






// void stacktrace( intframe_t *frame )
// {
//     syslog log("stacktrace");
//     intframe_t *sf = frame;

//     for (int i=0; i<16; i++)
//     {
//         log("rip: 0x%lx", sf->rip);
//         log("rbp: 0x%lx\n", sf->rbp);
//         sf = (intframe_t*)(&sf->rbp);
//     }
// }

// static void invalidOpISR( intframe_t *frame )
// {
//     syslog log("Exception %u (%s)", frame->isrno, IntNoStr(frame->isrno));

//     auto *cpu = SMP::this_cpu();
//     if (cpu) log("cpu %d", cpu->id);

// }

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






