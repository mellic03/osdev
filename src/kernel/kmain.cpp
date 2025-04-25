#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kernel.h>
#include <kernel/clock.hpp>
#include <kernel/log.hpp>
#include <kernel/vfs.hpp>
#include <kinterrupt.h>
#include <kthread.hpp>

#include <string.h>
#include <stdio.h>

#include "boot/boot.hpp"
#include "cpu/gdt.hpp"
#include "cpu/smp.hpp"
#include "driver/mouse.hpp"
#include "driver/pic.hpp"
#include "driver/pit.hpp"
#include "driver/keyboard.hpp"
#include "driver/serial.hpp"

#include <kinplace/inplace_vector.hpp>
#include <kscancode.h>


#include "./interrupt/interrupt.hpp"
#include "./kfs/kfs.hpp"
#include "./syscall/syscall.hpp"
#include "./ksystem.hpp"
#include "sde/sde.hpp"
#include "kshell/kshell.hpp"
#include "panic.hpp"


extern void LimineRes_init();

void pagefault_handler( kstackframe* );

void genfault_handler( kstackframe* )
{
    syslog::kprintf("Exception GENERAL_PROTECTION_FAULT");
    kernel_hcf();
}

void oom_handler( kstackframe* )
{
    syslog::kprintf("Exception OUT_OF_MEMORY");
    kernel_hcf();
}

void badfile_handler( kstackframe* )
{
    syslog::kprintf("Exception BAD_FILE");
    kernel_hcf();
}



#include "cpu/cpu.hpp"



#include <libc.h>
#include <libc++>

extern "C"
{
    using ctor_t = void (*)();
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


void pit_irq( kstackframe *frame )
{
    kclock::uptime_msecs.add_fetch(5);
    kthread::schedule(frame);
    PIT::reload();
}

extern void early_init();
extern void late_init();
extern void kpanic_init();


struct SMP_Info
{
    uint32_t cpu_idx;
    void (*start)();
} smp_info[16];


static void cpu0_main()
{
    uint32_t cpu_idx = SMP::get_lapic_id();
    syslog::kprintf("[cpu0_main] lapic_id=%u\n", cpu_idx);

    vfsInsertFile<char>("/dev/kb0/raw",   64, vfsFileFlag_Stream);
    vfsInsertFile<char>("/dev/kb0/event", 64, vfsFileFlag_Stream);
    vfsInsertFile<char>("/dev/ms0/raw",   64, vfsFileFlag_Stream);
    vfsInsertFile<char>("/dev/ms0/event", 64, vfsFileFlag_Stream);
    vfsInsertFile<char>("/dev/stdout",    1024, vfsFileFlag_Stream);
    vfsInsertDirectory("/dev/tty0/");

    // hwDriverInterface *hwDrivers[] = {
    //     new hwdi_PS2Mouse(),
    //     new hwdi_PS2Keyboard(),
    //     new hwdi_PIT(5)
    // };

    mouse_init();
    PIT::init();
    PIT::set_ms(5);
    GDT::load();
    GDT::flush();
    kernel::IDT_load();
    kernel::onInterrupt(INT_GENERAL_PROTECTION_FAULT,  genfault_handler);
    kernel::onInterrupt(INT_PAGE_FAULT,                pagefault_handler);
    kernel::onInterrupt(INT_BAD_FILE,                  badfile_handler);
    kernel::onInterrupt(INT_OUT_OF_MEMORY,             oom_handler);
    kernel::onInterrupt(INT_KTHREAD_START,             kthread::start_handler);
    kernel::onInterrupt(INT_KTHREAD_YIELD,             kthread::schedule);
    kernel::onInterrupt(INT_PANIC,                     kpanic_handler);
    kernel::onInterrupt(INT_SYSCALL,                   idk::syscall_handler);
    kernel::registerIRQ(0,                             pit_irq);
    kernel::registerIRQ(1,                             kdriver::ps2_kb::irq_handler);
    kernel::registerIRQ(12,                            kdriver::ps2_mouse::irq_handler);

    PIC::unmask(0);
    PIC::unmask(1);
    PIC::unmask(2);
    PIC::unmask(12);

    libc_init();
    libcpp::init();

    // for (auto *hwd: hwDrivers)
    // {
    //     hwd->loadIrqHandler();
    // }
	asm volatile ("sti");

    // std::vector<kthread*> threads;
    // for (auto *hwd: hwDrivers)
    // {
    //     if (!hwd->entry)
    //         continue;
    //     threads.push_back(new kthread(hwd->name, hwd->entry, nullptr));
    // }
    // threads.push_back(new kthread("SDE", sde_main, nullptr));
    // threads.push_back(new kthread("KShell", kshell_main, nullptr));

    kthread t0("ps2_mouse", kdriver::ps2_mouse::driver_main, nullptr);
    kthread t1("ps2_kb", kdriver::ps2_kb::driver_main, nullptr);
    kthread t3("SDE",    sde_main, nullptr);
    kthread t4("KShell", kshell_main, nullptr);
    kthread::start();

    while (true)
    {
        asm volatile ("hlt");
    }
}



static void SMP_processor_init( limine_mp_info *limineInfo )
{
    uint32_t idx = limineInfo->lapic_id;
    kernel::CPU::enableSSE(idx);
    syslog::kprintf("[SMP_processor_init] lapic_id=%u\n", idx);

    switch (idx)
    {
        default:
        case 0: return;
        case 1: gx_main(); return;
        case 2: break;
        case 3: break;
    }

    while (true) { asm volatile ("cli; hlt"); }
}



extern "C"
void _start()
{
    using namespace idk;
    asm volatile("cli");

    uint32_t boot_lapic = SMP::get_lapic_id();
    kernel::CPU::enableSSE(boot_lapic);

    idk::serial_init();
    early_init();
    call_ctors();
    late_init();

    syslog log("_start");
    log("boot_lapic:  %u\n", boot_lapic);

    auto *mpres = limine_res.mp;

    for (size_t i=0; i<mpres->cpu_count; i++)
    {
        auto *info = mpres->cpus[i];

        syslog lg("CPU %d", i);
        lg("processor_id: %u", info->processor_id);
        lg("lapic_id:     %u", info->lapic_id);
        lg("goto_address: 0x%lx", info->goto_address);
        info->extra_argument = (uint64_t)i;
    
        if (info->lapic_id != boot_lapic)
        {
            __atomic_store_n(&(info->goto_address), &SMP_processor_init, __ATOMIC_SEQ_CST);
        }
    }

    cpu0_main();

    while (true) { asm volatile ("cli; hlt"); }
}


void stacktrace( kstackframe *frame )
{
    syslog log("stacktrace");
    kstackframe *sf = frame;

    for (int i=0; i<16; i++)
    {
        log("rip: 0x%lx", sf->iret_rip);
        log("rbp: 0x%lx\n", sf->rbp);
        sf = (kstackframe*)(&sf->rbp);
    }
}


void pagefault_handler( kstackframe *frame )
{
    syslog log("Exception PAGE_FAULT");
    // log("thread %d (%s)", kthread::m_curr->tid, kthread::m_curr->name);

    log("rip:   0x%lx", frame->iret_rip);
    log("rsp:   0x%lx", frame->iret_rsp);
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
    log("vcode: %lu", frame->vcode);
    log("ecode: %lu", frame->ecode);

    uint64_t flags = frame->ecode;
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

    kernel_hcf();

}



