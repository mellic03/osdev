#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


    struct kstackframe
	{
		uint64_t r11, r12, r13, r14, r15;
		uint64_t rax, rbx, rcx, rdx;
		uint64_t rdi, rsi, rbp;
		uint64_t vcode, ecode;
	};


#ifdef __cplusplus
}
#endif

