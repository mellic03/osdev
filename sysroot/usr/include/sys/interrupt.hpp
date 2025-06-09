#pragma once
#include <stddef.h>
#include <stdint.h>
#include <cpu/idt.hpp>


struct intframe_t
{
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t rax, rbx, rcx, rdx, rdi, rsi, rbp;
    uint64_t isrno, errno;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));


enum IntNo_: uint8_t
{
    IntNo_ExceptionBase             = 0,
    IntNo_DivisionError             = 0,
    IntNo_Debug 			        = 1,
    IntNo_InvalidOpcode             = 6,
    IntNo_DeviceUnavailable         = 7,
    IntNo_DoubleFault               = 8,
    IntNo_InvalidTSS                = 10,
    IntNo_SegmentNotPresent         = 11,
    IntNo_StackSegfault             = 12,
    IntNo_GenFault                  = 13,
    IntNo_PageFault	  	            = 14,

    IntNo_x86_FP_Exception          = 16,
    IntNo_SIMD_FP_Exception         = 19,
    IntNo_ExceptionEnd              = 31,

    IntNo_BadAlloc 	  	            = 52,
    IntNo_BadFree  	  	            = 53,
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
        case IntNo_DivisionError: return "IntNo_DivisionError";
        case IntNo_Debug: return "IntNo_Debug";
        case IntNo_InvalidOpcode: return "IntNo_InvalidOpcode";
        case IntNo_DeviceUnavailable: return "IntNo_DeviceUnavailable";
        case IntNo_DoubleFault: return "IntNo_DoubleFault";
        case IntNo_InvalidTSS: return "IntNo_InvalidTSS";
        case IntNo_SegmentNotPresent: return "IntNo_SegmentNotPresent";
        case IntNo_StackSegfault: return "IntNo_StackSegfault";
        case IntNo_GenFault: return "IntNo_GenFault";
        case IntNo_PageFault: return "IntNo_PageFault";
        case IntNo_x86_FP_Exception: return "IntNo_x86_FP_Exception";
        case IntNo_SIMD_FP_Exception: return "IntNo_SIMD_FP_Exception";
        case IntNo_BadAlloc: return "IntNo_BadAlloc";
        case IntNo_BadFree: return "IntNo_BadFree";
        case IntNo_OUT_OF_MEMORY: return "IntNo_OUT_OF_MEMORY";
        case IntNo_KThreadYield: return "IntNo_KThreadYield";
        case IntNo_IrqBase: return "IntNo_IrqBase";
        case IntNo_IrqEnd: return "IntNo_IrqEnd";
        case IntNo_Spurious: return "IntNo_Spurious";
        case IntNo_Syscall: return "IntNo_Syscall";
    }
}

