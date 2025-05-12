#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kassert.h>
#include <kpanic.h>
#include <khang.h>
#include <kthread.hpp>
#include <new>

#include <kernel/boot_limine.hpp>
#include <kernel/clock.hpp>
#include <kernel/log.hpp>
#include <kernel/interrupt.hpp>
#include <kernel/input.hpp>
#include <kernel/syscall.h>
#include <kernel/memory/vmm.hpp>
#include <kernel/module.hpp>
#include <kernel/kvideo.hpp>

#include <cpu/cpu.hpp>
#include <cpu/scheduler.hpp>

#include <string.h>
#include <stdio.h>

#include <sys/acpi.hpp>
#include <driver/pic.hpp>
#include <driver/pci.hpp>
#include <driver/pit.hpp>
#include <driver/serial.hpp>

#include <filesystem/ramfs.hpp>
#include <filesystem/vfs2.hpp>
#include <filesystem/initrd.hpp>

#include "syscall/syscall.hpp"
#include "smp/smp.hpp"


static void genfaultISR( intframe_t* );
static void pagefaultISR( intframe_t* );
static void outOfMemoryISR( intframe_t* );


extern "C"
{
    using  ctor_t = void(*)();
    extern ctor_t __init_array_start[];
    extern ctor_t __init_array_end[];
}

void call_ctors()
{
    syslog log("call_ctors");
    int i = 0;
    for (ctor_t *ctor = __init_array_start; ctor < __init_array_end; ctor++)
    {
        log("ctor[%d]: 0x%lx", i++, ctor);
        (*ctor)();
    }
}


static void PIT_IrqHandler( intframe_t *frame )
{
    kclock::detail::tick(1);
    // syslog::println("[PIT_IrqHandler]");
    // syslog::println("[PIT_IrqHandler] CPU %u", SMP::this_cpu()->id);

    if (SMP::this_sched()->m_isRunning.load() == true)
        ThreadScheduler::scheduleISR(frame);

}


// external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/plugin/include
extern void LimineRes_init();
extern void early_init();
extern void late_init();
static void smp_main(limine_mp_info*);



// #include <atomic2>
// #include <smp/barrier.hpp>
std::atomic_int barrierA{4};
// sl::Atomic<int> barrierB(4);


// The volatile storage class was originally meant for memory-mapped I/O
// registers.  Within the kernel, register accesses, too, should be protected
// by locks, but one also does not want the compiler "optimizing" register
// accesses within a critical section.  But, within the kernel, I/O memory
// accesses are always done through accessor functions; accessing I/O memory
// directly through pointers is frowned upon and does not work on all
// architectures.  Those accessors are written to prevent unwanted
// optimization, so, once again, volatile is unnecessary.

// Another situation where one might be tempted to use volatile is
// when the processor is busy-waiting on the value of a variable.  The right
// way to perform a busy wait is::

//     while (my_variable != what_i_want)
//         cpu_relax();

// asm volatile ("" ::: "memory")

/*
    https://stackoverflow.com/questions/67943540/why-can-asm-volatile-memory-serve-as-a-compiler-barrier

    If a variable is potentially read or written, it matters what order that happens in.
    The point of a "memory" clobber is to make sure the reads and/or writes in an asm statement
    happen at the right point in the program's execution.
*/

void BarrierWait( std::atomic_int &barrier )
{
    barrier.fetch_sub(1, std::memory_order_seq_cst);

    while (barrier.load(std::memory_order_seq_cst) > 0)
    {
        // lol does not work in loop
        // asm volatile ("" ::: "memory");
        asm volatile ("nop");
    }
}



// std::atomic_int IRQ_disable_counter{0};

// void lock_scheduler(void)
// {
//     CPU::cli();
//     IRQ_disable_counter++;
// }

// void unlock_scheduler(void)
// {
//     if (IRQ_disable_counter-- == 0)
//         CPU::sti();
// }





__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request lim_rsdp_req = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 3
};



extern "C"
void _start()
{
    asm volatile ("cli");
    CPU::enableSSE();
    serial::init();
    syslog log("_start");

    early_init();
    call_ctors();

    CPU::clearIDT();
    ACPI::init((void*)(lim_rsdp_req.response->address));

    barrierA.store(4);
    SMP::initMulticore(smp_main);

    kassert(false); // Should be unreachable!

    kernel::halt();
}













// https://forum.osdev.org/viewtopic.php?t=57471
// static kthread_t      *task_list[64];
// static std::atomic_int task_count{0};
const auto &println = syslog::println;


// static void smp_hell( limine_mp_info* )
// {
//     while (true)
//     {
//         asm volatile ("cli; hlt");
//     }
// }


static void smp_main( limine_mp_info *info )
{
    asm volatile ("cli");
    CPU::enableSSE();
    CPU::createGDT();
    CPU::installGDT();
    size_t cpuid     = info->lapic_id;
    cpu_t *cpu       = new (SMP::all_cpus + cpuid) cpu_t();
           cpu->self = cpu;
           cpu->id   = cpuid;
    CPU::wrmsr(CPU::MSR_GS_BASE, (uint64_t)cpu);
    CPU::wrmsr(CPU::MSR_KERNEL_GS_BASE, (uint64_t)cpu);

    println("[CPU%u] BarrierA 0", cpuid);
    BarrierWait(barrierA);
    println("[CPU%u] BarrierA 1", cpuid);

    if (cpuid > 0)
    {
        kernel::hang();
    }

    PIC::remap(PIC::IRQ_MASTER, PIC::IRQ_SLAVE);
    PIC::disable();
    PIT::init(500);

    CPU::createIDT();
    CPU::installIDT();
    CPU::installISR(IntNo_GENERAL_PROTECTION_FAULT,  genfaultISR);
    CPU::installISR(IntNo_PAGE_FAULT,                pagefaultISR);
    CPU::installISR(IntNo_OUT_OF_MEMORY,             outOfMemoryISR);
    CPU::installISR(IntNo_KTHREAD_START,             ThreadScheduler::trampolineISR);
    CPU::installISR(IntNo_KTHREAD_YIELD,             ThreadScheduler::scheduleISR);
    CPU::installISR(IntNo_SYSCALL,                   kernel::syscallISR);
    CPU::installIRQ(IrqNo_PIT,                       PIT_IrqHandler);
    PIC::unmask(2);


    // https://github.com/jjwang/HanOS/blob/mainline/kernel/sys/acpi.c
    // https://github.com/jteerice/FrazzOS/blob/3e5492db5f8201a426807466153f9dbb2df7fdb3/kernel/src/firmware/acpi.c#L130
    // https://github.com/pdoane/osdev/blob/master/acpi/acpi.c

    // kthread::create("cullmain", kthread::cullmain, nullptr);

    if (cpuid == 0)
    {
        kernel::loadModules(initrd::find("drv/"));
        kernel::loadModules(initrd::find("srv/"));
        kernel::initModules();
    }

    asm volatile ("sti");
    PIC::unmask(IrqNo_PIT);

    // println("[CPU%u] APIC check: %u", cpuid, APIC::check());
    kthread::create("idlemain", kthread::idlemain, nullptr);
    kthread::start();

    kernel::halt();
}







// // Code A https://forum.osdev.org/viewtopic.php?t=57326
// // --------------------------------------------------
// struct CPU_t
// {
//     struct CPU_t *self;
//     int nr;
// };

// static CPU_t *cpu_self()
// {
//     CPU_t *r;
//     __asm__("mov %%gs:0, %0" : "=r"(r));
//     return r;
// }
// // --------------------------------------------------


// // Code B
// // --------------------------------------------------
// static CPU_t *cpu_self(void)
// {
//     return ((__seg_gs struct CPU_t *)0)->self;
// }
// // --------------------------------------------------









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


static void genfaultISR( intframe_t* )
{
    syslog::printf("Exception GENERAL_PROTECTION_FAULT");
    kernel::hang();
}

static void outOfMemoryISR( intframe_t* )
{
    syslog::printf("Exception OUT_OF_MEMORY");
    kernel::hang();
}


static std::atomic_int faultCount{0};

static void pagefaultISR( intframe_t *frame )
{
    syslog log("Exception PAGE_FAULT");

    // auto *cpu = SMP::this_cpu();
    // if (cpu) log("cpu %d", cpu->id);

    // auto *th = SMP::this_thread();
    // if (th) log("thread %d (%s)", th, th->name);

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






