#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define INTERRUPT_RET \
asm volatile ("iretq"); \
while (1) {  }; \


typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtr_t;


namespace idk
{
	struct Exception
	{
		// https://wiki.osdev.org/Exceptions
		enum enum_type: uint8_t
		{
			DIVISION_ERROR            = 0,
			DEBUG 			          = 1,
			INVALID_OPCODE            = 6,
			DEVICE_UNAVAILABLE        = 7,
			DOUBLE_FAULT              = 8,
			INVALID_TSS               = 10,
            SEGMENT_NOT_PRESENT       = 11,
            STACK_SEGFAULT            = 12,
            GENERAL_PROTECTION_FAULT  = 13,
			PAGE_FAULT	  	          = 14,

			BAD_ALLOC 	  	          = 32,
			BAD_FREE  	  	          = 33,
			OUT_OF_MEMORY 	          = 34,
    
			SYSCALL 	  	          = 80,
			TEST_VALUE		          = 102
		};
	};

	namespace interrupt
	{
		using isr_type = void (*)();

		void initIDT();
		void loadIDT();
		void registerSystemISR( uint8_t code, isr_type callback );
	}

	inline static constexpr
    void Interrupt( uint8_t code )
    {
        asm volatile(
            "int %0"
            :
            : "i" (code)
        );
    }
}


