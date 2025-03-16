#include "./system.hpp"

#include "drivers/kb-ps2.hpp"
#include "drivers/pic.hpp"
#include "drivers/pit.hpp"
#include "drivers/serial.hpp"
#include "gdt/gdt.hpp"
#include "idk_sysio/idk_sysio.hpp"
#include "io.hpp"
#include "memory/memory.hpp"
#include "video/video.hpp"

#include <idk/interrupt.hpp>
#include <stdio.h>


void __gen_protection_fault(); // ( uint64_t );
void __page_fault(); // ( uint64_t );
void __except_bad_alloc(); // ( uint64_t );
void __except_bad_free(); // ( uint64_t );
void __except_out_of_memory(); // ( uint64_t );
void __keyboard_handler(); // ( uint64_t );
void __irq0(); // ( uint64_t );
void __irq1(); // ( uint64_t );
void __irq2(); // ( uint64_t );
void __irq8(); // ( uint64_t );



idk::System&
idk::getSystem()
{
    static idk::System __idk_system;
    return __idk_system;
}



void
idk::System::init( const idk_BootInfo &info )
{
    cpu0.enableSSE();

    idk::memory::init(m_mainblock, info.mmap, info.hhdm->offset);
    idk::serial::init();
    SYSLOG_BEGIN("idk::System::init")
    SYSLOG("mem.base=0x%x, mem.length=%luKB\n", m_mainblock.m_base, m_mainblock.m_capacity/1024);
    SYSLOG_END

    idk_sysio_init(m_mainblock);

    sys::loadGDT();
    serial_printf("GDT loaded\n");

    {
        using namespace internal;
        using namespace interrupt;

        idk::DisableInterrupts();
        PIC::remap(40, 48);
        PIC::unmaskAll();
        interrupt::loadIDT();

        // // interrupt::registerSystemISR(0x01, __keyboard_handler);
        registerISR(Exception::GENERAL_PROTECTION_FAULT, __gen_protection_fault);
        registerISR(Exception::PAGE_FAULT,               __page_fault);
        registerISR(Exception::BAD_ALLOC,                __except_bad_alloc);
        registerISR(Exception::BAD_FREE,                 __except_bad_free);
        registerISR(Exception::OUT_OF_MEMORY,            __except_out_of_memory);
        registerISR(Exception::SYSCALL,                  __syscall_handler);

        interrupt::registerISR(40, __irq0);
        interrupt::registerISR(41, __irq1);
        interrupt::registerISR(42, __irq2);
        interrupt::registerISR(48, __irq8);
        // interrupt::registerISR(60, __rect_handler);
        // registerISR(8, __rect_handler);
        idk::EnableInterrupts();
    }


    internal::__syscall_init(m_mainblock);

    this->video.init(m_mainblock, info.fb_list[0]);
    this->video.loadFonts(info.modules);
    // this->scheduler.init();

    // PIT_init();

    // clock.uptime_ticks = 0;
    // clock.uptime_msec  = 0;

    m_terminals.init(32, m_mainblock.alloc<Terminal*>(32));
    tty0 = createTerminal(80, 25);
    tty1 = createTerminal(80, 25);

    // winroot = video.createWindow({0, 0}, {800, 500});
    // winroot->term = tty0;
    // winroot->font = &(video.getFonts())[6];

}


idk::System::System()
{

}

idk::System::~System()
{
    SYSLOG_BEGIN("idk::System::~System");
    // SYSLOG("total uptime: %lus\n", clock.uptime_msec/1000);
    SYSLOG_END
}





void
idk::System::update()
{
    // clock.update();
    // scheduler.update();
    video.update();
}






void
idk::SystemClock::update()
{
    // static uint64_t seconds = 0;

    // if (PIT_edge())
    // {
    //     uptime_ticks += PIT_HERTZ;
    //     uptime_msec  = uptime_ticks / PIT_HERTZ;

    //     uint64_t scnds = uptime_msec / 1000;
    
    //     if (scnds > seconds)
    //     {
    //         seconds = scnds;
    //         SYSLOG("seconds=%lu\n", seconds); // Print delta
    //     }
    // }
}




idk::Terminal*
idk::System::createTerminal( int w, int h )
{
    static int id = 0;
    m_terminals.push_back(new Terminal(id++, w, h, new char[w*h]));
    return m_terminals.back();
}








extern "C"
{
    extern void __enable_SSE(void);

    // void __idk_fxsave(void)
    // {
    //     idk::getSystem().cpu0.fxsave();
    // }

    // void __idk_fxrstor(void)
    // {
    //     idk::getSystem().cpu0.fxrstor();
    // }
}

void
idk::CPU::enableSSE()
{
    __enable_SSE();
    fxsave();
}


















void printBits( uint64_t value )
{
    for (int idx=63; idx>=0; idx--)
    {
        if ((value & (1<<idx)))
        {
            serial_printf("1");
        }
    
        else
        {
            serial_printf("0");
        }
    }

    serial_printf("\n");
}



void __gen_protection_fault() // ( uint64_t ecode )
{
    // SYSLOG("[__gen_protection_fault] ecode=%lu\n", ecode);
    SYSLOG("[__gen_protection_fault]\n");
    asm volatile ("cli; hlt");
}



void __page_fault() // ( uint64_t ecode )
{
    SYSLOG_BEGIN("__page_fault")

    // #define TITLE "[Exception PAGE_FAULT] "
    // SYSLOG(TITLE"ecode=%u\n", uint32_t(ecode));
    // syslog_pushIndent(strlen(TITLE));
    // SYSLOG("data=");


    // uint64_t flags = ecode;
    // printBits(flags);

    // if (flags & (1<<0))
    //     serial_printf("\t- Page-protection violation\n");
    // else serial_printf("\t- Non-present page\n");

    // if (flags & (1<<1))
    //      serial_printf("\t- Caused by write-access\n");
    // else serial_printf("\t- Caused by read-access\n");

    // if (flags & (1<<2))
    //     serial_printf("\t- Caused while CPL=3, not necessarily privilege violation\n");

    // if (flags & (1<<3))
    //     serial_printf("\t- One or more page directory entries contain reserved bits which are set to 1. This only applies when the PSE or PAE flags in CR4 are set to 1. \n");

    // if (flags & (1<<4))
    //     serial_printf("\t- Caused by instruction fetch\n");

    // if (flags & (1<<5))
    //     serial_printf("\t- Caused by a protection-key violation\n");

    // if (flags & (1<<6))
    //     serial_printf("\t- Caused by a shadow stack access\n");

    // if (flags & (1<<15))
    //     serial_printf("\t- Fault was due to an SGX violation. The fault is unrelated to ordinary paging.\n");

    SYSLOG_END

    asm volatile("hlt");
    while (1) {  };
}


void __except_bad_alloc() // ( uint64_t )
{
    serial_printf("[Exception::BAD_ALLOC]\n");
}

void __except_bad_free() // ( uint64_t )
{
    serial_printf("[Exception::BAD_FREE]\n");
}

void __except_out_of_memory() // ( uint64_t )
{
    serial_printf("[Exception::OUT_OF_MEMORY]\n");
}


void __keyboard_handler() // ( uint64_t )
{
    serial_printf("[__keyboard_handler]\n");
}


void __irq0() // ( uint64_t ecode )
{
    // SYSLOG("[__irq0] ecode=%lu\n", ecode);
    SYSLOG("[__irq0]\n");
    idk::PIC::sendEOI(0);
}

void __irq1() // ( uint64_t ecode )
{
    // SYSLOG("[__irq1] ecode=%lu\n", ecode);
    SYSLOG("[__irq1]\n");
    idk::PIC::sendEOI(1);
}

void __irq2() // ( uint64_t ecode )
{
    // SYSLOG("[__irq2] ecode=%lu\n", ecode);
    SYSLOG("[__irq2]\n");
    idk::PIC::sendEOI(2);
}

void __irq8() // ( uint64_t ecode )
{
    // SYSLOG("[__irq8] ecode=%lu\n", ecode);
    SYSLOG("[__irq8]\n");
    idk::PIC::sendEOI(8);
}