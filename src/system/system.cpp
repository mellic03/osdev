#include "./system.hpp"

#include "asm.hpp"
#include "drivers/kb-ps2.hpp"
#include "drivers/pic.hpp"
#include "drivers/serial.hpp"
#include "gdt/gdt.hpp"
#include "idk_sysio/idk_sysio.hpp"
#include "interrupt/interrupt.hpp"
#include "io.hpp"
#include "memory/memory.hpp"
#include "video/video.hpp"

#include <stdio.h>



// idk::System::System()
// :   value(0xDEADBEEF)
// {
//     // __sys_terminals = idk::inplace_vector<Terminal>(16, (Terminal*)(&__sys_buffer[0]));
// }


idk::System&
idk::getSystem()
{
    static idk::System __idk_system;
    return __idk_system;
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



void __gen_protection_fault( uint64_t ecode )
{
    SYSLOG("[__gen_protection_fault] ecode=%lu\n", ecode);
    asm volatile ("cli; hlt");
}



void __page_fault( uint64_t ecode )
{
    serial_printf("[Exception PAGE_FAULT] data=");

    uint64_t flags = ecode;
    printBits(flags);

    if (flags & (1<<0))
        serial_printf("\t- Page-protection violation\n");
    else serial_printf("\t- Non-present page\n");

    if (flags & (1<<1))
         serial_printf("\t- Caused by write-access\n");
    else serial_printf("\t- Caused by read-access\n");

    if (flags & (1<<2))
        serial_printf("\t- Caused while CPL=3, not necessarily privilege violation\n");

    if (flags & (1<<3))
        serial_printf("\t- One or more page directory entries contain reserved bits which are set to 1. This only applies when the PSE or PAE flags in CR4 are set to 1. \n");

    if (flags & (1<<4))
        serial_printf("\t- Caused by instruction fetch\n");

    if (flags & (1<<5))
        serial_printf("\t- Caused by a protection-key violation\n");

    if (flags & (1<<6))
        serial_printf("\t- Caused by a shadow stack access\n");

    if (flags & (1<<15))
        serial_printf("\t- Fault was due to an SGX violation. The fault is unrelated to ordinary paging.\n");
   
    asm volatile("hlt");
    while (1) {  };
}


void __except_bad_alloc()
{
    serial_printf("[Exception::BAD_ALLOC]\n");
}

void __except_bad_free()
{
    serial_printf("[Exception::BAD_FREE]\n");
}

void __except_out_of_memory()
{
    serial_printf("[Exception::OUT_OF_MEMORY]\n");
}


void __keyboard_handler()
{
    serial_printf("[__keyboard_handler]\n");
}




int
idk::System::init( const idk_BootInfo &info )
{
    #ifdef IDK_SSE
        cpu0.enableSSE();
    #endif

    if (!idk::memory::init(m_mainblock, info.mmap, info.hhdm->offset))
    {
        return 0;
    }

    if (!idk::serial::init(m_mainblock))
    {
        return 0;
    }

    #ifdef IDK_SSE
        SYSLOG("[idk::System::init] SSE Enabled\n");
    #else
        SYSLOG("[idk::System::init] SSE not available\n");
    #endif

    if (!this->video.init(m_mainblock, info.fb_list[0]))
    {
        return 0;
    }

    if (!idk_sysio_init(m_mainblock))
    {
        return 0;
    }

    m_terminals.init(32, m_mainblock.alloc<Terminal*>(32));

    sys::loadGDT();
    serial_printf("GDT loaded\n");
    
    {
        using namespace internal;

        interrupt::loadIDT();
        // // interrupt::registerSystemISR(0x01, __keyboard_handler);
        // interrupt::registerISR(Exception::GENERAL_PROTECTION_FAULT, __gen_protection_fault);
        // interrupt::registerISR(Exception::PAGE_FAULT,    __page_fault);
        // interrupt::registerISR(Exception::BAD_ALLOC,     __except_bad_alloc);
        // interrupt::registerISR(Exception::BAD_FREE,      __except_bad_free);
        // interrupt::registerISR(Exception::OUT_OF_MEMORY, __except_out_of_memory);
        interrupt::registerExceptionHandler(Exception::SYSCALL, __syscall_handler);
    }
    serial_printf("IDT loaded\n");

    internal::__syscall_init(m_mainblock);

    return 1;
}




idk::Terminal*
idk::System::createTerminal( int w, int h )
{
    static int id = 0;
    auto &buddy = memory::getBuddy();
    m_terminals.push_back(new Terminal(id++, w, h, buddy.alloc<char>(w*h)));
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
