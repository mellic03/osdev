#include "./kmain-requests.cpp"


#include <cxxabiv1>
#include <kernel.h>
#include <kinterrupt.h>

#include <kdriver/pic.hpp>
#include <kdriver/ps2_keyboard.hpp>
#include <kdriver/serial.hpp>
#include <kinplace/inplace_vector.hpp>
#include <kmemory/memory.hpp>
#include <kernel/cpu.hpp>
#include <kvideo/video.hpp>

#include "./cpu/gdt.hpp"
#include "./interrupt/interrupt.hpp"
#include "./lang/lang.hpp"
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

    idk::KRequests lim_reqs = {
        .hhdm    = lim_hhdm_req.response,
        .fb      = lim_fb_req.response,
        .modules = lim_module_req.response,
        .mmaps   = lim_mmap_req.response,
        .mp      = lim_mp_req.response
    };
    
    idk::KSystem system(lim_reqs);

    PIC::remap(60, 68);
    PIC::unmaskAll();
    asm volatile("cli");

    idk::GDT_load();
    idk::GDT_flush();
    idk::IDT_load();
    idk::onInterrupt(INT_GENERAL_PROTECTION_FAULT, genfault_handler);
    idk::onInterrupt(INT_PAGE_FAULT, pagefault_handler);
    idk::onInterrupt(INT_SYSCALL, idk::syscall_handler);
    // idk::EnableInterrupts();

    system.video.init(lim_reqs.fb);


    auto file = system.getModule("hello_world.exec");
    idk::ExecHeader header = { file->address, file->size };
    int result = system.execute(&header, 0, nullptr);
    SYSLOG("\n\nexec result: 0x%lx\n\n", uint32_t(result));


    auto &video = system.video;
    auto *win = video.createWinFrame<idk::WinFrameSolid>(
        vec2(100, 100), vec2(400, 500)
    );

    win->m_bg = uvec4(255, 155, 155, 255);
    win->m_depth = 0.5f;

    ps2_keyboard kb;

    while (true)
    {
        video.update();

        char key = kb.poll();

        if (kb.state['w']) { win->m_tl.y -= 1; }
        if (kb.state['s']) { win->m_tl.y += 1; }
        if (kb.state['a']) { win->m_tl.x -= 1; }
        if (kb.state['d']) { win->m_tl.x += 1; }

        // if (win->m_tl.x > 100)
        // {
        //     idk::Interrupt<INT_SYSCALL>();
        // }

        if (!key)
        {
            continue;
        }
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


    SYSLOG_END();
}
