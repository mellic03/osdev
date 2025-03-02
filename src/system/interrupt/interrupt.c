#include "interrupt.h"

interrupt_desc_t idt[256];
extern char vector_0_handler[];


void init()
{
    for (size_t i = 0; i < 256; i++)
    {
        set_idt_entry(i, (uint64_t)vector_0_handler + (i * 16), 0);
    }
}


void set_idt_entry( uint8_t vector, void* handler, uint8_t dpl )
{
    uint64_t handler_addr = (uint64_t)handler;

    interrupt_desc_t *entry = &idt[vector];
    entry->address_low = handler_addr & 0xFFFF;
    entry->address_mid = (handler_addr >> 16) & 0xFFFF;
    entry->address_high = handler_addr >> 32;

    //your code selector may be different!
    entry->selector = 0x08;


    //trap gate + present + DPL
    entry->flags = 0b1110 | ((dpl & 0b11) << 5) |(1 << 7);

    //ist disabled
    entry->ist = 0;
}



void load_idt( void *addr )
{
    idtr_t idt_reg;
    idt_reg.limit = 0x0FFF; // long mode length
    idt_reg.base  = (uint64_t)addr;
    asm volatile("lidt %0" :: "m"(&idt_reg));
}



cpu_status_t*
interrupt_dispatch( cpu_status_t *ctx )
{
    switch (ctx->vector_number)
    {
        case 13:    log("general protection fault.");   break;
        case 14:    log("page fault.");                 break;
        default:    log("unexpected interrupt.");       break;
    }

    return ctx;
}