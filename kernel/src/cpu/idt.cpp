#include <arch/apic.hpp>
#include <cpu/cpu.hpp>
#include <cpu/idt.hpp>
#include <kprintf.hpp>
#include <kpanic.hpp>

#include <sys/interrupt.hpp>
#include <kernel/log.hpp>
#include <driver/pic.hpp>

#include <kmemxx.hpp>

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
    {
        usrtab[isrno](frame);
    }

    if ((IntNo_IrqBase <= isrno) && (isrno <= IntNo_IrqEnd))
    {
        LAPIC::sendEOI();
    }
}


static void defaultExceptionHandler( intframe_t *frame )
{
    syslog log("defaultExceptionHandler");

    log("rax:    0x%lx", frame->rax);
    log("rbx:    0x%lx", frame->rbx);
    log("rcx:    0x%lx", frame->rcx);
    log("rdx:    0x%lx", frame->rdx);
    log("rdi:    0x%lx", frame->rdi);
    log("rsi:    0x%lx", frame->rsi);
    log("rbp:    0x%lx", frame->rbp);
    log("isrno:  %lu",   frame->isrno);
    log("errno:  %lu",   frame->errno);
    log("rip:    0x%lx", frame->rip);
    log("cs:     0x%lx", frame->cs);
    log("rflags: 0x%lx", frame->rflags);
    log("rsp:    0x%lx", frame->rsp);
    log("ss:     0x%lx", frame->ss);

    kpanic("%s", IntNoStr(frame->isrno));
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
#define TRAP_GATE 0x8


void CPU::createIDT()
{
    kmemset<uint8_t>(usrtab, 0, sizeof(usrtab));

    for (size_t i=0; i<NUM_INTERRUPTS; i++)
    {
        idt_setdesc(idt_entries[i], (uintptr_t)(isrtab[i]), INTERRUPT_GATE);
        usrtab[i] = defaultExceptionHandler;
    }

    idtr.base  = (uint64_t)(&idt_entries[0]);
    idtr.limit = (uint16_t)sizeof(idt_entries) - 1;

    // for (size_t i=IntNo_ExceptionBase; i<IntNo_ExceptionEnd; i++)
    //     usrtab[i] = defaultExceptionHandler;

    // kprintf("[CPU::createIDT] base=0x%lx, limit=%lu\n", idtr.base, idtr.limit);
}

void CPU::installIDT()
{
    asm volatile ("lidt %0" :: "m"(idtr));
}

void CPU::installISR( uint8_t isrno, isrHandlerFn handler )
{
    usrtab[isrno] = handler;
}

void CPU::installIRQ( uint8_t irqno, irqHandlerFn handler )
{
    usrtab[IntNo_IrqBase + irqno] = handler;
}

