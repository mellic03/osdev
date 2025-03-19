#pragma once
#include <AAA.h>


enum Kintcode: uint8_t
{
    INT_DIVISION_ERROR            = 0,
    INT_DEBUG 			          = 1,
    INT_INVALID_OPCODE            = 6,
    INT_DEVICE_UNAVAILABLE        = 7,
    INT_DOUBLE_FAULT              = 8,
    INT_INVALID_TSS               = 10,
    INT_SEGMENT_NOT_PRESENT       = 11,
    INT_STACK_SEGFAULT            = 12,
    INT_GENERAL_PROTECTION_FAULT  = 13,
    INT_PAGE_FAULT	  	          = 14,

    INT_BAD_ALLOC 	  	          = 52,
    INT_BAD_FREE  	  	          = 53,
    INT_OUT_OF_MEMORY 	          = 54,

    INT_KEYBOARD				  = 90,
    INT_SYSCALL 	  	          = 99
};


namespace idk
{
	template <uint8_t vcode>
	void Interrupt()
	{
		asm volatile(
			"int %0"
			:
			: "i" (vcode)
		);
	}


	inline static constexpr
    void EnableInterrupts()
    {
        asm volatile("sti");
    }


	inline static constexpr
    void DisableInterrupts()
    {
        asm volatile("cli");
    }

}






#include <AAB.h>
