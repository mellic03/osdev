#ifndef __is_kernel
    #define __is_kernel
#endif

#include "kmain-requests.cpp"

#include <cxxabiv1>
#include <kernel.h>
#include <kinterrupt.h>

#include <libc.h>
#include <libc++>

#include "driver/pic.hpp"
#include "driver/pit.hpp"
// #include "driver/ps2_keyboard.hpp"
#include "driver/keyboard.hpp"
#include "driver/serial.hpp"

#include <kinplace/inplace_vector.hpp>

#include <kscancode.h>

#include "./cpu/gdt.hpp"
#include "./interrupt/interrupt.hpp"
#include "./kfs/kfs.hpp"
// #include "./lang/lang.hpp"
#include "./syscall/syscall.hpp"
#include "./ksystem.hpp"



void pagefault_handler( kstackframe* );

void genfault_handler( kstackframe *frame )
{
    SYSLOG_BEGIN("Exception GENERAL_PROTECTION_FAULT");
    kernel_hcf();
    SYSLOG_END();
}



uint64_t idk::memory::hhdm;

void asm_yolo( void *addr )
{
    void (*yolo)(void) = (void (*)())addr;
    yolo();
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














#include <kstackframe.h>


struct CPU_Task
{
    uint8_t stack[1024];
    kstackframe state;
    CPU_Task *next;
};


CPU_Task *curr_task;
CPU_Task *next_task;


void task_init()
{
    curr_task = new CPU_Task();
    next_task = new CPU_Task();

    curr_task->next = next_task;
    next_task->next = curr_task;

    cpu_ctx_save(&(curr_task->state));
    cpu_ctx_save(&(next_task->state));
}

void task_switch()
{
    cpu_ctx_save(&(curr_task->state));
    curr_task = curr_task->next;
    cpu_ctx_load(&(curr_task->state));
}



















idk::KSystem *sys;
idk::WinFrameSolid *win;
uint64_t count;

void pit_irq( kstackframe* )
{
    count++;
    idk::PIT_reload();
    idk::PIC::sendEOI(0);
}




























#include <string.h>





extern "C"
void _start()
{
    run_init_array();

    using namespace idk;

    if (!idk::serial_init())
    {
        return;
    }

    SYSLOG_BEGIN("_start");
    SYSLOG("serial initialized");
    
    Krequests reqs = {
        .hhdm    = lim_hhdm_req.response->offset,
        .fb      = lim_fb_req.response,
        .modules = lim_module_req.response,
        .mmaps   = lim_mmap_req.response,
        .mp      = lim_mp_req.response
    };

    idk::KSystem system(reqs);
    sys = &system;
    count = 0;
    double timer = 0;


    asm volatile("cli");
    idk::PIT_init();
    idk::GDT_load();
    idk::GDT_flush();
    idk::IDT_load();
    idk::onInterrupt(INT_GENERAL_PROTECTION_FAULT, genfault_handler);
    idk::onInterrupt(INT_PAGE_FAULT, pagefault_handler);
    idk::onInterrupt(INT_SYSCALL, idk::syscall_handler);
    idk::onInterrupt(0x20+0,  pit_irq);
    idk::onInterrupt(0x20+1,  keyboard_irq_handler);

    PIC::remap(32, 40);
    PIC::disable();
    PIC::unmask(0);
    PIC::unmask(1);
	asm volatile ("sti");


    system.video.init(system.m_reqs.fb);

    libc_init();
    std::detail::libcpp_init();

    KFS::Trie trie;
    auto *E = trie.insert("/home/michael/Desktop/sketchy.exec");
    E->print_path();

    // auto file = system.getModule("kshell.exec");
    // idk::ExecHeader header = { file->address, file->size };
    // int result = system.execute(&header, 0, nullptr);
    // SYSLOG("\n\nexec result: 0x%lx\n\n", result);

    // void  *exec_addr = (void*)(0xFFFF800100000000);
    // memcpy(exec_addr, header.addr, header.size);
    // using YOLO = int (*)(int, char**);
    // int retvalue = ((YOLO)exec_addr)(0, nullptr);
    // // return retvalue;



    auto &video = system.video;
    win = video.createWinFrame<idk::WinFrameSolid>(
        vec2(100, 100), vec2(400, 500)
    );

    win->m_bg = uvec4(255, 155, 155, 255);
    win->m_depth = 0.5f;


    // uint64_t ticks = 0;
    // uint64_t msecs = 0;
    // static uint64_t seconds = 0;

    while (true)
    {
        timer = double(count);
        SYSLOG("timer=%f", timer/1000.0);


        // if (PIT_edge())
        // {
        //     ticks += PIT_HERTZ;
        //     msecs = ticks / 1000;
        //     uint64_t scnds = msecs / 1000;
        
        //     if (scnds > seconds)
        //     {
        //         seconds = scnds;
        //         SYSLOG("seconds=%lu\n", seconds); // Print delta
        //     }
        // }

        video.update();
        uint8_t key = 0;

        while (KFile_read(&key, KFS::kdevscn, 1))
        {
            if (key==DOWN_W) win->m_tl.y -= 1;
            if (key==DOWN_S) win->m_tl.y += 1;
            if (key==DOWN_A) win->m_tl.x -= 1;
            if (key==DOWN_D) win->m_tl.x += 1;
        }

        // if (win->m_tl.x > 100)
        // {
        //     KInterrupt<INT_SYSCALL>();
        // }
    }



    SYSLOG_END();
    run_fini_array();
    kernel_hcf();
}




void pagefault_handler( kstackframe *frame )
{
    SYSLOG_BEGIN("Exception PAGE_FAULT");

    uint64_t flags = frame->ecode;
    // printBits(flags);

    if (flags & (1<<0))
         SYSLOG("\t- Page-protection violation");
    else SYSLOG("\t- Non-present page");

    if (flags & (1<<1))
         SYSLOG("\t- Caused by write-access");
    else SYSLOG("\t- Caused by read-access");

    if (flags & (1<<2))
        SYSLOG("\t- Caused while CPL=3, not necessarily privilege violation");

    if (flags & (1<<3))
        SYSLOG("\t- One or more page directory entries contain reserved bits which are set to 1. This only applies when the PSE or PAE flags in CR4 are set to 1. ");

    if (flags & (1<<4))
        SYSLOG("\t- Caused by instruction fetch");

    if (flags & (1<<5))
        SYSLOG("\t- Caused by a protection-key violation");

    if (flags & (1<<6))
        SYSLOG("\t- Caused by a shadow stack access");

    if (flags & (1<<15))
        SYSLOG("\t- Fault was due to an SGX violation. The fault is unrelated to ordinary paging.");

    kernel_hcf();

    SYSLOG_END();
}



