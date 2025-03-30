#ifndef __is_kernel
    #define __is_kernel
#endif

#include "boot/requests.cpp"

#include <kernel.h>
#include <kernel/vfs.hpp>
#include <kernel/log.hpp>
#include <kinterrupt.h>
#include <kthread.hpp>

#include <string.h>
#include <stdio.h>

#include "driver/mouse.hpp"
#include "driver/pic.hpp"
#include "driver/pit.hpp"
#include "driver/keyboard.hpp"
#include "driver/serial.hpp"

#include <kinplace/inplace_vector.hpp>
#include <kscancode.h>

#include "./cpu/smp.hpp"
#include "./cpu/gdt.hpp"
#include "./interrupt/interrupt.hpp"
#include "./kfs/kfs.hpp"
#include "./syscall/syscall.hpp"
#include "./ksystem.hpp"
#include "kvideo/kvideo.hpp"
#include "sde/sde.hpp"
#include "kshell/kshell.hpp"
#include "panic.hpp"


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


idk::KSystem *sys;

namespace kclock
{
    uint64_t uptime_msecs = 0;
    double   uptime_secs  = 0.0;

};


void pit_irq( kstackframe *frame )
{
    kclock::uptime_msecs += 2;
    kclock::uptime_secs  += 1000.0 / double(PIT_HERTZ);
    kthread::schedule(frame);

    PIT::reload();
    PIC::sendEOI(0);
}


extern "C"
{
    extern void __kthread_test( void );
}


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
    for (ctor_t *ctor = __init_array_start; ctor < __init_array_end-1; ctor++)
    {
        log("ctor[%d]: 0x%lx %lu", i++, ctor, ctor);
        (*ctor)();
    }
}




extern "C"
void _start()
{
    using namespace idk;    
    asm volatile("cli");

    // syslog::disable();
    idk::serial_init();
    syslog log("_start");
    log("serial initialized");

    Krequests reqs = {
        .hhdm    = lim_hhdm_req.response->offset,
        .fb      = lim_fb_req.response,
        .addr    = lim_addr_req.response,
        .modules = lim_module_req.response,
        .mmaps   = lim_mmap_req.response,
        .mp      = lim_mp_req.response
    };

    idk::KSystem system(reqs);
    sys = &system;
    call_ctors();
    SMP::init(reqs.mp);
 

    mouse_init();
    PIT::init();
    PIT::set_ms(2);
    GDT::load();
    GDT::flush();
    idk::IDT_load();


    idk::onInterrupt(INT_GENERAL_PROTECTION_FAULT, genfault_handler);
    idk::onInterrupt(INT_PAGE_FAULT, pagefault_handler);
    idk::onInterrupt(INT_BAD_FILE,      badfile_handler);
    idk::onInterrupt(INT_OUT_OF_MEMORY, oom_handler);
    idk::onInterrupt(INT_KTHREAD_START, kthread::start_handler);
    idk::onInterrupt(INT_KTHREAD_YIELD, kthread::schedule);
    idk::onInterrupt(INT_PANIC,   kpanic_handler);
    idk::onInterrupt(INT_SYSCALL, idk::syscall_handler);
    idk::onInterrupt(32+0,  pit_irq);
    idk::onInterrupt(32+1,  kdriver::ps2_kb::irq_handler);
    idk::onInterrupt(32+12, kdriver::ps2_mouse::irq_handler);
    PIC::remap(32, 40);
    PIC::disable();
    PIC::unmask(0);
    PIC::unmask(1);
    PIC::unmask(2);
    PIC::unmask(12);
	asm volatile ("sti");


    kfilesystem::vfsInsertFile<char>("/dev/kb0/raw",   64, vfsFileFlag_Stream);
    kfilesystem::vfsInsertFile<char>("/dev/kb0/event", 64, vfsFileFlag_Stream);
    kfilesystem::vfsInsertFile<char>("/dev/ms0/raw",   64, vfsFileFlag_Stream);
    kfilesystem::vfsInsertFile<char>("/dev/ms0/event", 64, vfsFileFlag_Stream);
    kfilesystem::vfsInsertFile<char>("/dev/stdout",    1024, vfsFileFlag_Stream);
    kvideo::init((uintptr_t)(reqs.fb));
    libc_init();
    libcpp::init();

    for (size_t i=0; i<reqs.modules->module_count; i++)
    {
        auto *F = reqs.modules->modules[i];
        kfilesystem::vfsInsertFile(F->path, F->address, F->size);
    }

    kTTY tty0(25*80);
    auto *file = kfilesystem::vfsFindFile("/font/cutive-w12hf18.bmp");
          file->flags |= vfsFileFlag_Stream;
          file->flags |= vfsFileFlag_Virtual;
    tty0.font = new idk::FontBuffer((ck_BMP_header*)(file->addr));

    kthread t0(kdriver::ps2_mouse::driver_main, nullptr);
    kthread t1(kdriver::ps2_kb::driver_main, nullptr);
    kthread t2(sde_main, nullptr);
    kthread t3(kshell_main, (void*)&tty0);
    kthread::start();

    while (true)
    {
        asm volatile ("hlt");
    }

    kernel_hcf();
}


void stacktrace( kstackframe *frame )
{
    syslog log("stacktrace");

    kstackframe *sf = frame;

    while (sf->rbp != 0)
    {
        log("rbp:   0x%lx", sf->rbp);
        sf = (kstackframe*)(sf->rbp);
    }
}


void pagefault_handler( kstackframe *frame )
{
    syslog log("Exception PAGE_FAULT");
    // stacktrace(frame);

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



