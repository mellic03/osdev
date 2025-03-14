#include "./system.hpp"

#include "drivers/kb-ps2.hpp"
#include "drivers/pic.hpp"
#include "drivers/serial.hpp"
#include "gdt/gdt.hpp"
#include "idk_sysio/idk_sysio.hpp"
#include "interrupt/interrupt.hpp"
#include "io.hpp"
#include "memory/memory.hpp"
#include "syscall/syscall.hpp"
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


inline static constexpr
uint64_t asm_popq()
{
    uint64_t value;

    asm volatile(
        "pop %0"
        : "=r" (value)
    );

    return value;
}


__attribute__((noreturn))
void __page_fault()
{
    uint64_t flags = asm_popq();
    serial_printf("[Exception PAGE_FAULT] data=");
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
    // INTERRUPT_RET;
}


// __attribute__((noreturn))
void __except_bad_alloc()
{
    serial_printf("[Exception::BAD_ALLOC]\n");
    // INTERRUPT_RET;
}

// __attribute__((noreturn))
void __except_bad_free()
{
    serial_printf("[Exception::BAD_FREE]\n");
    // INTERRUPT_RET;
}

// __attribute__((noreturn))
void __except_out_of_memory()
{
    serial_printf("[Exception::OUT_OF_MEMORY]\n");
    // INTERRUPT_RET;
}


void __keyboard_handler()
{
    using namespace idk;

    auto &cpu0 = getSystem().cpu0;
    cpu0.fxsave();

    uint8_t scancode = IO::inb(0x60);
    serial_printf("[__keyboard_handler] scancode=%u\n", scancode);

    cpu0.fxrstor();
}


void __except_rect()
{
    using namespace idk;
    auto &cpu0 = getSystem().cpu0;
    cpu0.fxsave();

    serial_printf("[__except_rect]\n");

    cpu0.fxrstor();

    // IO::outb(PIC::PIC1_CMD, 0x20);
    // asm volatile ("iretq");
}



int
idk::System::init( const idk_BootInfo &info )
{
    cpu0.enableSSE();

    if (!idk::memory::init(m_mainblock, info.mmap, info.hhdm->offset))
    {
        return 0;
    }

    if (!idk::serial::init(m_mainblock))
    {
        return 0;
    }

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
        interrupt::initIDT();
        // interrupt::registerSystemISR(0x01, __keyboard_handler);
        interrupt::registerSystemISR(Exception::PAGE_FAULT,    __page_fault);
        interrupt::registerSystemISR(Exception::BAD_ALLOC,     __except_bad_alloc);
        interrupt::registerSystemISR(Exception::BAD_FREE,      __except_bad_free);
        interrupt::registerSystemISR(Exception::OUT_OF_MEMORY, __except_out_of_memory);
        interrupt::registerSystemISR(Exception::SYSCALL,       __syscall_handler);
        interrupt::registerSystemISR(Exception::TEST_VALUE,    __except_rect);
        interrupt::loadIDT();
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
}

void
idk::CPU::enableSSE()
{
    __enable_SSE();
    fxsave();
}
