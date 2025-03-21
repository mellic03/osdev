#pragma once
#include <AAA.h>


struct kstackframe
{
    uint64_t r11, r12, r13, r14, r15;
    uint64_t rax, rbx, rcx, rdx;
    uint64_t rdi, rsi, rbp;
    uint64_t vcode, ecode;
};


#include <ZZZ.h>
