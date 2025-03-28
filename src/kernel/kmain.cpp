#ifndef __is_kernel
    #define __is_kernel
#endif

#include "abi/cxxabi.hpp"

#include "boot/requests.cpp"

#include <kernel.h>
#include <kinterrupt.h>
#include <kthread.hpp>

#include <string.h>
#include <stdio.h>

#include "driver/pic.hpp"
#include "driver/pit.hpp"
#include "driver/keyboard.hpp"
#include "driver/serial.hpp"
#include "log/log.hpp"

#include <kinplace/inplace_vector.hpp>

#include <kscancode.h>

#include "./cpu/gdt.hpp"
#include "./interrupt/interrupt.hpp"
#include "./kfs/kfs.hpp"
#include "./syscall/syscall.hpp"
#include "./ksystem.hpp"
#include "kvideo/kvideo.hpp"
#include "kwin/kwin.hpp"
#include "kshell/kshell.hpp"


#include "driver/mouse.hpp"


void pagefault_handler( kstackframe* );

void genfault_handler( kstackframe* )
{
    syslog("Exception GENERAL_PROTECTION_FAULT");
    kernel_hcf();
}

void oom_handler( kstackframe* )
{
    syslog("Exception OUT_OF_MEMORY");
    kernel_hcf();
}



uint64_t idk::memory::hhdm;
idk::KSystem *sys;
uint64_t uptime_ms;

void pit_irq( kstackframe *frame )
{
    kthread::schedule(frame);
    uptime_ms += 10;

    idk::PIT_reload();
    idk::PIC::sendEOI(0);
}



#include "kwin/frame_tty.hpp"
#include "kwin/frame_text.hpp"

bool rect_point_overlap( vec2 tl, vec2 sp, vec2 p )
{
    bool x = p.x == std::clamp(p.x, tl.x, tl.x+sp.x);
    bool y = p.y == std::clamp(p.y, tl.y, tl.y+sp.y);
    return x && y;
}





extern "C"
{
    using constructor_t = void (*)();
    extern constructor_t __init_array_start[];
    extern constructor_t __init_array_end[];

    void ctor_init( void )
    {
        syslog log("ctor_init");

        for (constructor_t *ctor = __init_array_start; ctor < __init_array_end-1; ctor++)
        {
            log("ctor: 0x%lx", ctor);
            (*ctor)();
        }
    }
}



extern "C"
void _start()
{
    using namespace idk;    

    if (!idk::serial_init())
    {
        return;
    }

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
    asm volatile("cli");

    idk::KSystem system(reqs);
    sys = &system;
    ctor_init();
    uptime_ms = 0;

    KFS::insertFile("dev/kb0/", "raw",   new kfstream(64));
    KFS::insertFile("dev/kb0/", "event", new kfstream(64));
    KFS::insertFile("dev/kb1/", "raw",   new kfstream(64));
    KFS::insertFile("dev/kb1/", "event", new kfstream(64));

    KFS::insertFile("dev/ms0", "raw",   new kfstream(64));
    KFS::insertFile("dev/ms0", "event", new kfstream(64));
    KFS::insertFile("dev/ms1", "raw",   new kfstream(64));
    KFS::insertFile("dev/ms1", "event", new kfstream(64));

    mouse_init();
    idk::PIT_init();
    idk::PIT_set_ms(2);
    idk::GDT_load();
    idk::GDT_flush();
    idk::IDT_load();
    idk::onInterrupt(INT_GENERAL_PROTECTION_FAULT, genfault_handler);
    idk::onInterrupt(INT_PAGE_FAULT, pagefault_handler);
    idk::onInterrupt(INT_KTHREAD_YIELD, kthread::schedule);
    idk::onInterrupt(INT_SYSCALL, idk::syscall_handler);
    idk::onInterrupt(INT_OUT_OF_MEMORY, oom_handler);
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

    for (auto *F: system.getModules())
    {
        size_t len = strlen(F->string);
        if (len == 0)
            continue;

        else if (strncmp(F->path, "/data/exec", 10) == 0)
            KFS::insertFile("bin/", F->string, F->address, F->size);

        else if (strncmp(F->path, "/data/font", 10) == 0)
            KFS::insertFile("font/", F->string, F->address, F->size);
    }

    kTTY tty0(25*80);
    tty0.font = &system.m_fonts[0];

    kthread t0(kdriver::ps2_mouse::driver_main, nullptr);
    kthread t1(kdriver::ps2_kb::driver_main, nullptr);
    kthread t2(kwin_main, (void*)(reqs.fb));
    kthread t3(kshell_main, (void*)&tty0);


    while (true)
    {
        asm volatile ("hlt");
    }

    kernel_hcf();
}




void pagefault_handler( kstackframe *frame )
{
    syslog log("Exception PAGE_FAULT");

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



