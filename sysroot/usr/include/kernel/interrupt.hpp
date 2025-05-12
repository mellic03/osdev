#pragma once
#include <stddef.h>
#include <stdint.h>
#include <cpu/idt.hpp>

#ifdef __cplusplus
extern "C" {
#endif



struct intframe_t
{
    uint64_t r11, r12, r13, r14, r15;
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
    IntNo_GENERAL_PROTECTION_FAULT  = 13,
    IntNo_PAGE_FAULT	  	        = 14,

    IntNo_BAD_ALLOC 	  	        = 52,
    IntNo_BAD_FREE  	  	        = 53,
    IntNo_OUT_OF_MEMORY 	        = 54,

    IntNo_KTHREAD_START             = 96,
    IntNo_KTHREAD_YIELD             = 97,
    IntNo_SYSCALL 	  	            = 110,
};

enum IrqNo_: uint8_t
{
    IrqNo_PIT                   = 0,
    IrqNo_Keyboard              = 1,
    IrqNo_Cascade               = 2,
    IrqNo_COM2                  = 3,
    IrqNo_COM1                  = 4,
    IrqNo_LPT2                  = 5,
    IrqNo_FloppyDisk            = 6,
    IrqNo_Spurious              = 7,
    IrqNo_CMOS                  = 8,
    IrqNo_Free9                 = 9,
    IrqNo_Free10                = 10,
    IrqNo_Free11                = 11,
    IrqNo_Mouse                 = 12,
    IrqNo_FPU                   = 13,
    IrqNo_PrimaryATA            = 14,
    IrqNo_SecondaryATA          = 15,
};




#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
    using isrHandlerFn = void (*)(intframe_t*);
    using irqHandlerFn = void (*)(intframe_t*);

    template <uint16_t isrno>
    constexpr inline void KInterrupt() { asm volatile("int %0" : : "i" (isrno)); }

#else
    typedef void (*isrHandlerFn)(intframe_t*);
    typedef void (*irqHandlerFn)(intframe_t*);

#endif




