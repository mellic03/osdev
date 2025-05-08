#pragma once
#include "AAA.h"


struct kstackframe
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
};



#include "ZZZ.h"
