#ifndef __is_kernel
    #define __is_kernel
#endif

#include "kmain-requests.cpp"

#include <cxxabiv1>
#include <kernel.h>
#include <kinterrupt.h>
#include <kproc.hpp>

#include <libc.h>
#include <libc++>

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
#include "kshell.hpp"
#include "memory/pmm.hpp"
#include "memory/vmm.hpp"


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
idk::Video   *vid;
uint64_t count;




#include <string.h>
#include <stdio.h>

void pit_irq( kstackframe *frame )
{
    kproc_switch(frame);
    // kproc_yield();

    count++;
    idk::PIT_reload();
    idk::PIC::sendEOI(0);
}


void proccess_switch_handler( kstackframe *frame )
{
    kproc_switch(frame);
}



#include "kwin/frame_tty.hpp"

void proc_video( void* )
{
    printf("[proc_video] A\n");

    auto ctx = kwin::Context(1280, 720);
    auto *frame = ctx.createFrame<kwin::FrameTest>(
        ivec2(100, 100), ivec2(400, 500), uvec4(255, 0, 255, 255)
    );

    // auto *frame = ctx.createFrame<kwin::FrameTTY>(
    //     ivec2(100, 100), ivec2(400, 500)
    // );

    while (true)
    {
        frame->draw(ctx);
        ctx.m_tl.x += 1;

        ctx.flush();
    }

}


void proc_text( void* )
{
    printf("[proc_text] A\n");

    while (true)
    {
        printf("[proc_text]\n");
        // kproc_yield();
    }
}





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

    // sch = &(system.sched);
    count = 0;
    double timer = 0;


    asm volatile("cli");
    kproc_init();

    idk::PIT_init();
    idk::GDT_load();
    idk::GDT_flush();
    idk::IDT_load();
    idk::onInterrupt(INT_GENERAL_PROTECTION_FAULT, genfault_handler);
    idk::onInterrupt(INT_PAGE_FAULT, pagefault_handler);
    idk::onInterrupt(INT_PROCESS_SWITCH, proccess_switch_handler);
    idk::onInterrupt(INT_SYSCALL, idk::syscall_handler);
    idk::onInterrupt(32+0,  pit_irq);
    idk::onInterrupt(32+1,  keyboard_irq_handler);

    PIC::remap(32, 40);
    PIC::disable();
    PIC::unmask(0);
    PIC::unmask(1);

    libc_init();
    std::detail::libcpp_init();
	asm volatile ("sti");

    system.video.init(system.m_reqs.fb);
    kvideo::init(reqs.fb);

    auto &video = system.video;
    vid = &video;

    auto *A = kproc_new(proc_video, nullptr);
    auto *B = kproc_new(kshell, nullptr); // needs to be blocking


    while (true)
    {
        // kproc_yield();
        // printf("WOOP\n");
    }

    uint64_t ree = 0xffff'8000'0000'0000;

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



