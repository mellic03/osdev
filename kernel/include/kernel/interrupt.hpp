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

    uint64_t iret_rip;
    uint64_t iret_cs;
    uint64_t iret_flags;
    uint64_t iret_rsp;
    uint64_t iret_ss;
} __attribute__((packed));


enum Int_: uint8_t
{
    Int_DIVISION_ERROR            = 0,
    Int_DEBUG 			          = 1,
    Int_INVALID_OPCODE            = 6,
    Int_DEVICE_UNAVAILABLE        = 7,
    Int_DOUBLE_FAULT              = 8,
    Int_INVALID_TSS               = 10,
    Int_SEGMENT_NOT_PRESENT       = 11,
    Int_STACK_SEGFAULT            = 12,
    Int_GENERAL_PROTECTION_FAULT  = 13,
    Int_PAGE_FAULT	  	          = 14,

    Int_BAD_ALLOC 	  	          = 52,
    Int_BAD_FREE  	  	          = 53,
    Int_OUT_OF_MEMORY 	          = 54,

    Int_KTHREAD_START             = 96,
    Int_KTHREAD_YIELD             = 97,
    Int_SYSCALL 	  	          = 99
};




#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
    using isrHandlerFn = void (*)(intframe_t*);
    using irqHandlerFn = void (*)(intframe_t*);

    template <uint8_t isrno>
    constexpr inline void KInterrupt() { asm volatile("int %0" : : "i" (isrno)); }

#else
    typedef void (*isrHandlerFn)(intframe_t*);
    typedef void (*irqHandlerFn)(intframe_t*);

#endif




