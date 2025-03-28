#ifndef __is_kernel
    #define __is_kernel
#endif

#include "boot/requests.cpp"

#include <cxxabiv1>
#include <kernel.h>
#include <kinterrupt.h>
#include <kproc.hpp>


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



void pagefault_handler( kstackframe* );

void genfault_handler( kstackframe *frame )
{
    syslog("Exception GENERAL_PROTECTION_FAULT");
    kernel_hcf();
}





extern "C" {
    // Symbols defined by the linker script
    extern void (**__init_array_start)(void);
    extern void (**__init_array_end)(void);
    extern void (**__fini_array_start)(void);
    extern void (**__fini_array_end)(void);
}

void run_init_array()
{
    for (void (**func)(void) = __init_array_start; func < __init_array_end; ++func)
    {
        (*func)();
    }
}

void run_fini_array()
{
    for (void (**func)(void) = __fini_array_start; func < __fini_array_end; ++func)
    {
        (*func)();
    }
}




uint64_t idk::memory::hhdm;
idk::KSystem *sys;
uint64_t uptime_ms;


#include <string.h>
#include <stdio.h>
#include "driver/mouse.hpp"

void pit_irq( kstackframe *frame )
{
    kthread::schedule(frame);
    // kthread::yield();

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


extern void video_main( void* );

void mouse_main( void* )
{
    while (true)
    {
        ProcessMousePacket();
        kthread::yield();
    }

}




#include "memory/heap_allocator.hpp"


extern "C"
void _start()
{
    run_init_array();

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

    idk::KSystem system(reqs);
    sys = &system;

    uptime_ms = 0;
    double timer = 0;


    asm volatile("cli");
    mouse_init();
    idk::PIT_init();
    idk::PIT_set_ms(2);
    idk::GDT_load();
    idk::GDT_flush();
    idk::IDT_load();
    idk::onInterrupt(INT_GENERAL_PROTECTION_FAULT, genfault_handler);
    idk::onInterrupt(INT_PAGE_FAULT, pagefault_handler);
    idk::onInterrupt(INT_PROCESS_SWITCH, kthread::schedule);
    idk::onInterrupt(INT_SYSCALL, idk::syscall_handler);
    idk::onInterrupt(32+0,  pit_irq);
    idk::onInterrupt(32+1,  kdriver::ps2_kb::irq_handler);
    idk::onInterrupt(32+12, mouse_irq);
    PIC::remap(32, 40);
    PIC::disable();
    PIC::unmask(0);
    PIC::unmask(1);
    PIC::unmask(2);
    PIC::unmask(12);

	asm volatile ("sti");

    u64vec2 data2(
        (uintptr_t)reqs.fb, (uintptr_t)sys
    );

    // kproc_new(tty_main, system.tty0);
    kthread t0(mouse_main, nullptr);
    kthread t1(video_main, &data2);
    kthread t2(kdriver::ps2_kb::driver_main, nullptr);

    while (true)
    {
        asm volatile ("hlt");
    }

    run_fini_array();
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



