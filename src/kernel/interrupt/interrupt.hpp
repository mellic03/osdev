#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kstackframe.h>


struct idt_entry_t
{
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed));


struct idtr_t
{
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed));




namespace kernel
{
	static constexpr uint8_t IRQ_MASTER = 32;
	static constexpr uint8_t IRQ_SLAVE  = 40;
	
    void IDT_load();
	void onInterrupt( uint8_t vcode, void (*handler)(kstackframe*) );
	void registerIRQ( uint8_t irqno, void (*handler)(kstackframe*) );
}


extern "C"
{
	extern uint64_t syscall_arg;
}
