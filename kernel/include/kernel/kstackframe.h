#pragma once
#include <kernel/interrupt.hpp>


using kstackframe = intframe_t;

// struct kstackframe
// {
//     uint64_t r11, r12, r13, r14, r15;
//     uint64_t rax, rbx, rcx, rdx;
//     uint64_t rdi, rsi, rbp;
//     uint64_t isrno, errno;

//     uint64_t rip;
//     uint64_t cs;
//     uint64_t rflags;
//     uint64_t rsp;
//     uint64_t ss;
// };
