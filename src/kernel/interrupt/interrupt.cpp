#include "interrupt.hpp"
#include "../driver/serial.hpp"
#include "../driver/pic.hpp"
#include "../cpu/gdt.hpp"

using namespace idk;
static constexpr size_t NUM_INTERRUPTS = 100;
using isr_type = void (*)(kstackframe*);

idt_entry_t  idt[256] __attribute__((aligned(0x10))) ; 
idtr_t       idtr;
isr_type     __usr_table[NUM_INTERRUPTS];


extern "C"
{
    extern void *__isr_table[];
}



extern "C"
void __isr_dispatch( kstackframe *frame )
// void __isr_dispatch( uint64_t vcode, uint64_t ecode )
{
    // SYSLOG_BEGIN("__interrupt_dispatch");

    uint64_t vcode = frame->vcode;
    uint64_t ecode = frame->ecode;

    // SYSLOG("r11:   0x%lx", frame->r11);
    // SYSLOG("r12:   0x%lx", frame->r12);
    // SYSLOG("r13:   0x%lx", frame->r13);
    // SYSLOG("r14:   0x%lx", frame->r14);
    // SYSLOG("r15:   0x%lx", frame->r15);
    // SYSLOG("rax:   0x%lx", frame->rax);
    // SYSLOG("rbx:   0x%lx", frame->rbx);
    // SYSLOG("rcx:   0x%lx", frame->rcx);
    // SYSLOG("rdx:   0x%lx", frame->rdx);
    // SYSLOG("rdi:   0x%lx", frame->rdi);
    // SYSLOG("rsi:   0x%lx", frame->rsi);
    // SYSLOG("rbp:   0x%lx", frame->rbp);
    // SYSLOG("vcode: %lu", vcode);
    // SYSLOG("ecode: %lu", ecode);

    if (vcode > NUM_INTERRUPTS)
    {
        // SYSLOG("vcode > NUM_INTERRUPTS");
    }

    if (__usr_table[vcode])
    {
        __usr_table[vcode](frame);
    }

    else
    {
        // SYSLOG("No handler");
    }

    // SYSLOG_END();
}





void idt_set_descriptor( uint8_t idx, void *isr, uint8_t flags )
{
    idt_entry_t* descriptor = &idt[idx];

    descriptor->isr_low    = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs  = GDT_OFFSET_KERNEL_CODE;
    descriptor->ist        = 0;
    descriptor->attributes = flags;
    descriptor->isr_mid    = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high   = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved   = 0;
}



#define INTERRUPT_GATE 0x8E
#define TRAP_GATE 0x8F

void idk::IDT_load()
{
    SYSLOG_BEGIN("idk::IDT_load");
    
    for (int i=0; i<NUM_INTERRUPTS; i++)
    {
        __usr_table[i] = nullptr;
    }

    idtr.base = (uint64_t)(&idt[0]);
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUM_INTERRUPTS - 1;

    SYSLOG("idtr.base:  0x%x", idtr.base);
    SYSLOG("idtr.limit: %u", idtr.limit);

    for (uint8_t i=0; i<NUM_INTERRUPTS; i++)
    {
        idt_set_descriptor(i, __isr_table[i], INTERRUPT_GATE);
    }

    asm volatile ("lidt %0" : : "m"(idtr)); // Load IDT

    SYSLOG_END();
}


void idk::onInterrupt( uint8_t vcode, void (*callback)(kstackframe*) )
{
    if (vcode > NUM_INTERRUPTS)
    {

    }

    else
    {
        __usr_table[vcode] = callback;
    }
}

