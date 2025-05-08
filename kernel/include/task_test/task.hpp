#pragma once
#include <stdint.h>


struct taskframe_t
{
	uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp;
	uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t rflags; 
	uint64_t rip; 
	uint64_t cr3;
}__attribute__((packed));

