#include <cpu/cpu.hpp>
#include <cpu/idt.hpp>

#include <kernel/interrupt.hpp>
#include <kernel/log.hpp>
#include <driver/pic.hpp>


static constexpr size_t NUM_INTERRUPTS = 256;
static idt_entry_t  idt_entries[NUM_INTERRUPTS] __attribute__((aligned(0x10))) ; 
static idt_ptr_t    idtr;
static isrHandlerFn usrtab[NUM_INTERRUPTS];

extern "C"
{
    extern void *isrtab[];
}


extern "C"
void isr_dispatch( intframe_t *frame )
{
    uint32_t isrno = frame->isrno;

    if (usrtab[isrno])
        usrtab[isrno](frame);

    if (PIC::IRQ_MASTER <= isrno && isrno <= PIC::IRQ_SLAVE+12)
        PIC::sendEOI(isrno - PIC::IRQ_MASTER);
}


void idt_setdesc( idt_entry_t &desc, uintptr_t isr, uint8_t flags )
{
    desc.isr_high   = (isr >> 32) & 0xFFFFFFFF;
    desc.isr_mid    = (isr >> 16) & 0xFFFF;
    desc.isr_low    =  isr & 0xFFFF;

    desc.kernel_cs  = CPU::GDT_OFFSET_KERNEL_CODE;
    desc.ist        = 0;
    desc.attributes = flags;
    desc.reserved   = 0;
}



#define INTERRUPT_GATE 0x8E
#define TRAP_GATE 0x8F


void CPU::createIDT()
{
    memset(usrtab, 0, sizeof(usrtab));
    for (size_t i=0; i<NUM_INTERRUPTS; i++)
    {
        // if (i >= PIC::IRQ_MASTER && i <= PIC::IRQ_SLAVE+12)
        //     idt_setdesc(idt_entries[i], (uintptr_t)(isrtab[i]), TRAP_GATE);
        // else
        idt_setdesc(idt_entries[i], (uintptr_t)(isrtab[i]), INTERRUPT_GATE);
    }
    idtr.base  = (uint64_t)(&idt_entries[0]);
    idtr.limit = (uint16_t)sizeof(idt_entries) - 1;
}


// void CPU::createIDT( idt_entry_t *idtbase, idt_ptr_t *idtptr )
// {
//     for (size_t i=0; i<NUM_INTERRUPTS; i++)
//         idt_setdesc(idtbase[i], (uintptr_t)(isrtab[i]), INTERRUPT_GATE);
//     idtptr->base  = (uint64_t)(&idtbase[0]);
//     idtptr->limit = (uint16_t)sizeof(idtbase) - 1;
// }


void CPU::installIDT()
{
    asm volatile ("lidt %0" :: "m"(idtr));
}


// void CPU::installIDT( idt_ptr_t idtptr )
// {
//     asm volatile ("lidt %0" :: "m"(idtptr));
// }


void CPU::installISR( uint8_t isrno, isrHandlerFn handler )
{
    usrtab[isrno] = handler;
}

void CPU::installIRQ( uint8_t irqno, irqHandlerFn handler )
{
    usrtab[PIC::IRQ_MASTER + irqno] = handler;
}

