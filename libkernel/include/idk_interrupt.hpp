#pragma once
#include <AAA.h>
#include <idk_interrupt.h>

namespace idk
{
	inline static constexpr
    void Interrupt( uint8_t vcode )
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
