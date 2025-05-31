#pragma once
#include <stddef.h>
#include <stdint.h>
#include <cpu/idt.hpp>


struct intframe_t
{
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t rax, rbx, rcx, rdx;
    uint64_t rdi, rsi, rbp;
    uint64_t isrno, errno;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));

enum IntNo_: uint8_t
{
    IntNo_DIVISION_ERROR            = 0,
    IntNo_DEBUG 			        = 1,
    IntNo_INVALID_OPCODE            = 6,
    IntNo_DEVICE_UNAVAILABLE        = 7,
    IntNo_DOUBLE_FAULT              = 8,
    IntNo_INVALID_TSS               = 10,
    IntNo_SEGMENT_NOT_PRESENT       = 11,
    IntNo_STACK_SEGFAULT            = 12,
    IntNo_GEN_FAULT                 = 13,
    IntNo_PAGE_FAULT	  	        = 14,
    
    IntNo_x86_FP_Exception          = 16,
    IntNo_SIMD_FP_Exception         = 19,
    // IntNo_TripleFault               = 

    IntNo_BAD_ALLOC 	  	        = 52,
    IntNo_BAD_FREE  	  	        = 53,
    IntNo_OUT_OF_MEMORY 	        = 54,

    IntNo_KThreadYield              = 97,

    IntNo_IrqBase                   = 100,
    IntNo_IrqEnd                    = IntNo_IrqBase + 24,

    IntNo_Spurious                  = 254,
    IntNo_Syscall 	  	            = 255,
};


enum IrqNo_: uint8_t
{
    IrqNo_PIT          = 0,
    IrqNo_Keyboard     = 1,
    IrqNo_Cascade      = 2,
    IrqNo_COM2         = 3,
    IrqNo_COM1         = 4,
    IrqNo_LPT2         = 5,
    IrqNo_FloppyDisk   = 6,
    IrqNo_Spurious     = 7,
    IrqNo_CMOS         = 8,
    IrqNo_Free9        = 9,
    IrqNo_Free10       = 10,
    IrqNo_Free11       = 11,
    IrqNo_Mouse        = 12,
    IrqNo_FPU          = 13,
    IrqNo_PrimaryATA   = 14,
    IrqNo_SecondaryATA = 15,
};



using isrHandlerFn = void (*)(intframe_t*);
using irqHandlerFn = void (*)(intframe_t*);

namespace knl
{
    template <uint16_t isrno>
    constexpr inline void interrupt()
    {
        asm volatile("int %0" : : "i" (isrno));
    }
}







inline const char *IntNoStr( uint8_t isrno )
{
    switch (isrno)
    {
        default: return "IntNo_Invalid";
        case IntNo_DIVISION_ERROR: return "IntNo_DIVISION_ERROR";
        case IntNo_DEBUG: return "IntNo_DEBUG";
        case IntNo_INVALID_OPCODE: return "IntNo_INVALID_OPCODE";
        case IntNo_DEVICE_UNAVAILABLE: return "IntNo_DEVICE_UNAVAILABLE";
        case IntNo_DOUBLE_FAULT: return "IntNo_DOUBLE_FAULT";
        case IntNo_INVALID_TSS: return "IntNo_INVALID_TSS";
        case IntNo_SEGMENT_NOT_PRESENT: return "IntNo_SEGMENT_NOT_PRESENT";
        case IntNo_STACK_SEGFAULT: return "IntNo_STACK_SEGFAULT";
        case IntNo_GEN_FAULT: return "IntNo_GEN_FAULT";
        case IntNo_PAGE_FAULT: return "IntNo_PAGE_FAULT";
        case IntNo_x86_FP_Exception: return "IntNo_x86_FP_Exception";
        case IntNo_SIMD_FP_Exception: return "IntNo_SIMD_FP_Exception";
        case IntNo_BAD_ALLOC: return "IntNo_BAD_ALLOC";
        case IntNo_BAD_FREE: return "IntNo_BAD_FREE";
        case IntNo_OUT_OF_MEMORY: return "IntNo_OUT_OF_MEMORY";
        case IntNo_KThreadYield: return "IntNo_KThreadYield";
        case IntNo_IrqBase: return "IntNo_IrqBase";
        case IntNo_IrqEnd: return "IntNo_IrqEnd";
        case IntNo_Spurious: return "IntNo_Spurious";
        case IntNo_Syscall: return "IntNo_Syscall";
    }
}

