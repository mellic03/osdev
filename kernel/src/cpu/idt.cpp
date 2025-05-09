// #include "idt.hpp"
#include <cpu/cpu.hpp>
#include <cpu/idt.hpp>

#include <kernel/interrupt.hpp>
#include <kernel/log.hpp>
#include <driver/pic.hpp>


static constexpr size_t NUM_INTERRUPTS = 255;
static constexpr size_t HANDLERS_PER_ISRNO = 4;
// using isrDispatchTable_t = idk::static_vector<isrHandlerFn, MAX_HANDLERS_PER_ISRNO>;

static idt_entry_t  idt_entries[256] __attribute__((aligned(0x10))) ; 
static idt_ptr_t    idtr;

static size_t       usr_count[NUM_INTERRUPTS];
static isrHandlerFn usr_handlers[NUM_INTERRUPTS][HANDLERS_PER_ISRNO];

extern "C"
{
    extern void *isr_table[]; // idt.asm
}


extern "C"
void isr_dispatch( intframe_t *frame )
{
    uint32_t isrno = frame->isrno;

    for (size_t i=0; i<HANDLERS_PER_ISRNO; i++)
    {
        if (usr_handlers[isrno][i])
        {
            usr_handlers[isrno][i](frame);
        }
    }

    // if (usr_table[isrno])
    // {
    //     usr_table[isrno](frame);
    // }

    if (PIC::IRQ_MASTER <= isrno && isrno <= PIC::IRQ_SLAVE+12)
    {
        PIC::sendEOI(isrno);
    }

}


void idt_setdesc( uint8_t idx, uintptr_t isr, uint8_t flags )
{
    auto &desc = idt_entries[idx];

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
    // syslog log("CPU::createIDT");

    for (size_t i=0; i<NUM_INTERRUPTS; i++)
    {
        for (size_t j=0; j<HANDLERS_PER_ISRNO; j++)
            usr_handlers[i][j] = nullptr;
        usr_count[i] = 0;

        idt_setdesc(i, (uintptr_t)(isr_table[i]), INTERRUPT_GATE);
    }

    idtr.base  = (uint64_t)(&idt_entries[0]);
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUM_INTERRUPTS - 1;

    // log("idtr.base:  0x%x", idtr.base);
    // log("idtr.limit: %u", idtr.limit);
}


void CPU::installIDT()
{
    // syslog log("kernel::installIDT");
    asm volatile ("lidt %0" :: "m"(idtr));
}


void CPU::installISR( uint8_t isrno, isrHandlerFn handler )
{
    if (usr_count[isrno] < HANDLERS_PER_ISRNO)
    {
        size_t idx = usr_count[isrno]++;
        usr_handlers[isrno][idx] = handler;
    }
}


void CPU::installIRQ( uint8_t irqno, irqHandlerFn handler )
{
    CPU::installISR(PIC::IRQ_MASTER + irqno, handler);
}

