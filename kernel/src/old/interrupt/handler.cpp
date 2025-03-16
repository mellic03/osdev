#include "interrupt.hpp"
#include "../gdt/gdt.hpp"
#include "../drivers/serial.hpp"
#include "system/drivers/pic.hpp"
#include "../system.hpp"
#include "../io.hpp"
// #include "cpu.hpp"

using namespace idk;
using usr_type = interrupt::handler_type;
extern usr_type usr_table[idk::NUM_INTERRUPTS];

extern "C"
{
    extern void *__isr_table[];
}



struct CPU_STATE
{
    uint64_t r15, r14, r13, r12, rbp, rbx;
    uint64_t r11, r10, r9, r8, rax, rcx, rdx, rsi, rdi;
    uint64_t vcode, ecode;
    uint64_t rip, cs, eflags, rsp, ss;

} __attribute__((packed));




static const char *get_str( uint8_t vcode )
{
    switch (vcode)
    {
        default:                                    break;
        case Exception::DIVISION_ERROR:             return "DIVISION_ERROR";
        case Exception::DEBUG: 			            return "DEBUG";
        case Exception::INVALID_OPCODE:             return "INVALID_OPCODE";
        case Exception::DEVICE_UNAVAILABLE:         return "DEVICE_UNAVAILABLE";
        case Exception::DOUBLE_FAULT:               return "DOUBLE_FAULT";
        case Exception::INVALID_TSS:                return "INVALID_TSS";
        case Exception::SEGMENT_NOT_PRESENT:        return "SEGMENT_NOT_PRESENT";
        case Exception::STACK_SEGFAULT:             return "STACK_SEGFAULT";
        case Exception::GENERAL_PROTECTION_FAULT:   return "GENERAL_PROTECTION_FAULT";
        case Exception::PAGE_FAULT:	  	            return "PAGE_FAULT";
        case Exception::BAD_ALLOC: 	  	            return "BAD_ALLOC";
        case Exception::BAD_FREE:  	  	            return "BAD_FREE";
        case Exception::OUT_OF_MEMORY: 	            return "OUT_OF_MEMORY";
        case Exception::KEYBOARD:				    return "KEYBOARD";
        case Exception::SYSCALL: 	  	            return "SYSCALL";
    };

    return "Unnamed Exception";
}



void asm_jmp( void *addr )
{
    void (*yolo)(void) = (void (*)())addr;
    yolo();
}


extern "C"
void __interrupt_dispatch( CPU_STATE *frame )
{
    uint64_t vcode = frame->vcode; // frame.vcode;
    uint64_t ecode = frame->ecode; // frame.ecode;

    SYSLOG("\n");
    SYSLOG_BEGIN("__interrupt_dispatch");

    SYSLOG("Exception: %s\n", get_str(uint8_t(vcode)));
    SYSLOG("vcode=%u\n", uint32_t(vcode));
    SYSLOG("ecode=%u\n", uint32_t(ecode));

    if (vcode >= idk::NUM_INTERRUPTS)
    {
        SYSLOG("Error, vcode >= NUM_INTERRUPTS\n");
    }

    else if (usr_table[vcode])
    {
        SYSLOG("calling handler...\n");
        (usr_table[vcode])(ecode);
    }

    else
    {
        SYSLOG("no handler\n");
    }

    SYSLOG_END

    asm_jmp(__isr_table[vcode]);

    // asm volatile ("cli; hlt");
}