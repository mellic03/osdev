// #pragma once

// #include <stdbool.h>
// #include <stddef.h>
// #include <stdint.h>

// struct idt_entry_t
// {
//     uint16_t    isr_low;      // The lower 16 bits of the ISR's address
//     uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
//     uint8_t     ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
//     uint8_t     attributes;   // Type and attributes; see the IDT page
//     uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
//     uint32_t    isr_high;     // The higher 32 bits of the ISR's address
//     uint32_t    reserved;     // Set to zero
// } __attribute__((packed));


// // struct idt_entry_t
// // {
// //     uint16_t offset1;
// //     uint16_t segmentSelector;
// //     uint8_t ist : 3;
// //     uint8_t reserved0 : 5;
// //     uint8_t gateType : 4;
// //     uint8_t empty : 1;
// //     uint8_t dpl : 2;
// //     uint8_t present : 1;
// //     uint16_t offset2;
// //     uint32_t offset3;
// //     uint32_t reserved;
// // } __attribute__((packed));


// typedef struct {
//     uint16_t    limit;
//     uint64_t    base;
// } __attribute__((packed)) idtr_t;

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



struct IDTEFrame
{
    uint64_t cr2;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t vcode;
    uint64_t ecode;
    uint64_t rip;
    uint64_t cs;
    uint64_t flags;
    uint64_t rsp;
    uint64_t ss;
};


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
	static constexpr size_t NUM_INTERRUPTS = 100;

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

			BAD_ALLOC 	  	          = 52,
			BAD_FREE  	  	          = 53,
			OUT_OF_MEMORY 	          = 54,

			KEYBOARD				  = 90,
			SYSCALL 	  	          = 99
		};
	};

	namespace interrupt
	{
		using isr_type = void (*)();
		using handler_type = void (*)(uint64_t);

		void loadIDT();
		void registerExceptionHandler( uint8_t vcode, handler_type );
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


