// #include "interrupt.hpp"
// #include "gdt/gdt.hpp"
// #include "drivers/serial.hpp"
// #include "drivers/pic.hpp"
// #include "io.hpp"
// #include "system.hpp"


// using namespace idk;
// using isr_type = interrupt::isr_type;
// using usr_type = interrupt::usr_type;

// extern "C"
// {
//     // extern void *__isr_table[];
//     // extern void divideException();
//     // extern void debugException();
//     // extern void breakpointException();
//     // extern void overflowException();
//     // extern void boundRangeExceededException();
//     // extern void invalidOpcodeException();
//     // extern void deviceNotAvaliableException();
//     // extern void doubleFaultException();
//     // extern void coprocessorSegmentOverrunException();
//     // extern void invalidTSSException();
//     // extern void segmentNotPresentException();
//     // extern void stackSegmentFaultException();
//     // extern void generalProtectionFaultException();
//     // extern void pageFaultException();
//     // extern void floatingPointException();
//     // extern void alignmentCheckException();
//     // extern void machineCheckException();
//     // extern void simdFloatingPointException();
//     // extern void virtualisationException();
//     // extern void syscallInterrupt();
//     // extern void *divideException;
//     // extern void *debugException;
//     // extern void *breakpointException;
//     // extern void *overflowException;
//     // extern void *boundRangeExceededException;
//     // extern void *invalidOpcodeException;
//     // extern void *deviceNotAvaliableException;
//     // extern void *doubleFaultException;
//     // extern void *coprocessorSegmentOverrunException;
//     // extern void *invalidTSSException;
//     // extern void *segmentNotPresentException;
//     // extern void *stackSegmentFaultException;
//     // extern void *generalProtectionFaultException;
//     // extern void *pageFaultException;
//     // extern void *floatingPointException;
//     // extern void *alignmentCheckException;
//     // extern void *machineCheckException;
//     // extern void *simdFloatingPointException;
//     // extern void *virtualisationException;
//     // extern void *syscallInterrupt;
// }

// inline static constexpr size_t NUM_INTERRUPTS = 128;
// inline static idt_entry_t __idt[256] __attribute__((aligned(0x10))) ; 
// inline static idtr_t      __idtr;
// inline static usr_type    usr_table[NUM_INTERRUPTS];



// // void asm_jmp( void *addr )
// // {
// //     void (*yolo)(void) = (void (*)())addr;
// //     yolo();
// // }


// extern "C"
// {
//     // void __interrupt_dispatch( IDTEFrame frame )
//     void __interrupt_dispatch()
//     {
//         #define TITLE "[__interrupt_dispatch] "
//         #define IDENT "                   "

//         uint64_t vcode = 0; // frame.vcode;
//         uint64_t ecode = 0; // frame.ecode;

//         SYSLOG(IDENT"vcode=%u\n", uint32_t(vcode));
//         SYSLOG(IDENT"ecode=%u\n", uint32_t(ecode));

//         if (vcode >= NUM_INTERRUPTS)
//         {
//             SYSLOG(IDENT"Error, vcode >= NUM_INTERRUPTS\n");
//         }

//         // else if (usr_table[vcode])
//         // {
//         //     SYSLOG(IDENT"calling handler...\n\n");
//         //     // (usr_table[vcode])(ecode);
//         // }

//         else
//         {
//             SYSLOG(IDENT"no handler\n");
//         }
//         SYSLOG("\n");

//         asm volatile ("cli; hlt");
//     }
// }



// void
// interrupt::registerISR( uint8_t vcode, usr_type callback )
// {
//     usr_table[vcode] = callback;
// }


// void idt_set_descriptor( uint8_t vcode, void *isr, uint8_t flags )
// {
//     idt_entry_t* descriptor = &__idt[vcode];

//     descriptor->isr_low    = (uint64_t)isr & 0xFFFF;
//     descriptor->kernel_cs  = GDT_OFFSET_KERNEL_CODE;
//     descriptor->ist        = 0;
//     descriptor->attributes = flags;
//     descriptor->isr_mid    = ((uint64_t)isr >> 16) & 0xFFFF;
//     descriptor->isr_high   = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
//     descriptor->reserved   = 0;
// }

// // void isr_keyboard()
// // {
// //     unsigned char status = inb(0x64); // Read the status register of the keyboard controller
// //     if (status & 0x01) { // Check if the least significant bit (bit 0) is set, indicating data is available
// //         unsigned char scan_code = inb(0x60); // Read the scan code from the keyboard controller
// //         // Check if it's a key press event (high bit not set)
// //         if (!(scan_code & 0x80) && inScanf) {
// //             // Handle the key press event
// //             char combined = convertScancode(scan_code);
// //             if (combined != '\0') {
// //                 addCharToString(wholeInput, combined);
// //                 terminal_putchar(combined);
// //             }
// //        }
// //     }
// //     outb(0x20, 0x20); // Acknowledge interrupt from master PIC
// //     outb(0xa0, 0x20); // Acknowledge interrupt from slave PIC
// //     init_IRQ();
// // }



// void interrupt::init()
// {
//     __idtr.base = (uintptr_t)&__idt[0];
//     __idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUM_INTERRUPTS - 1;

//     for (uint8_t i=0; i<NUM_INTERRUPTS; i++)
//     {
//         usr_table[i] = nullptr;
//     }

//     // remapPIC();
//     // // idt_set_descriptor(Exception::TASK_SWITCH, &isr_taskswitch, 14, 0);
//     // // idt_set_descriptor(Exception::KEYBOARD, &isr_keyboard, 14, 0);
//     // idt_set_descriptor(Exception::SYSCALL, syscallInterrupt, INTERRUPT_GATE);
//     // unmaskIRQ(1); // keyboard
//     // unmaskIRQ(0); // pit

//     // idt_set_descriptor(0,  divideException, TRAP_GATE);
//     // idt_set_descriptor(1,  debugException, TRAP_GATE);
//     // idt_set_descriptor(3,  breakpointException, TRAP_GATE);
//     // idt_set_descriptor(4,  overflowException, TRAP_GATE);
//     // idt_set_descriptor(5,  boundRangeExceededException, TRAP_GATE);
//     // idt_set_descriptor(6,  invalidOpcodeException, TRAP_GATE);
//     // idt_set_descriptor(7,  deviceNotAvaliableException, TRAP_GATE);
//     // idt_set_descriptor(8,  doubleFaultException, TRAP_GATE);
//     // idt_set_descriptor(9,  coprocessorSegmentOverrunException, TRAP_GATE);
//     // idt_set_descriptor(10, invalidTSSException, TRAP_GATE);
//     // idt_set_descriptor(11, segmentNotPresentException, TRAP_GATE);
//     // idt_set_descriptor(12, stackSegmentFaultException, TRAP_GATE);
//     // idt_set_descriptor(13, generalProtectionFaultException, TRAP_GATE);
//     // idt_set_descriptor(14, pageFaultException, TRAP_GATE);
//     // idt_set_descriptor(16, floatingPointException, TRAP_GATE);
//     // idt_set_descriptor(17, alignmentCheckException, TRAP_GATE);
//     // idt_set_descriptor(18, machineCheckException, TRAP_GATE);
//     // idt_set_descriptor(19, simdFloatingPointException, TRAP_GATE);
//     // idt_set_descriptor(20, virtualisationException, TRAP_GATE);
//     // idt_set_descriptor(Exception::SYSCALL, syscallInterrupt, TRAP_GATE);

// }


// void interrupt::loadIDT()
// {

//     for (int i=0; i<NUM_INTERRUPTS; i++)
//     {
//         idt_set_descriptor(0, __isr_table[i], TRAP_GATE);
//     }


//     // idk::PIC::remap(0, 8);
//     // idk::IO::outb(0x0021, 0xFD);
//     // idk::IO::outb(0x00A1, 0xFF);
//     asm volatile ("cli");                     // Disable interrupts
//     asm volatile ("lidt %0" : : "m"(__idtr)); // Load IDT
//     asm volatile ("sti");                     // Enable interrupts

// }



#include "interrupt.hpp"
#include "gdt/gdt.hpp"
#include "drivers/serial.hpp"
#include "drivers/pic.hpp"
#include "system.hpp"
#include "io.hpp"
#include "log.hpp"

using namespace idk;
using isr_type = interrupt::isr_type;
using usr_type = interrupt::handler_type;

idt_entry_t idt[256] __attribute__((aligned(0x10))) ; 
idtr_t      idtr;
usr_type    usr_table[NUM_INTERRUPTS];

extern "C"
{
    extern void *__isr_table[];
}




void keyboard_handler( uint64_t ecode )
{
    SYSLOG("[keyboard_handler] ecode=%lu\n", ecode);
    // unsigned char status = IO::inb(0x64); // Read the status register of the keyboard controller
    // if (status & 0x01) { // Check if the least significant bit (bit 0) is set, indicating data is available
    //     unsigned char scan_code = IO::inb(0x60); // Read the scan code from the keyboard controller
    //     // Check if it's a key press event (high bit not set)
    //     if (!(scan_code & 0x80) && inScanf) {
    //         // Handle the key press event
    //         char combined = convertScancode(scan_code);
    //         if (combined != '\0') {
    //             addCharToString(wholeInput, combined);
    //             terminal_putchar(combined);
    //         }
    //    }
    // }
    // IO::outb(0x20, 0x20); // Acknowledge interrupt from master PIC
    // IO::outb(0xa0, 0x20); // Acknowledge interrupt from slave PIC
    // init_IRQ();
}



void
interrupt::registerExceptionHandler( uint8_t vcode, handler_type handler )
{
    usr_table[vcode] = handler;
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

void interrupt::loadIDT()
{
    SYSLOG_BEGIN("idk::interrupt::loadIDT");

    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUM_INTERRUPTS - 1;

    SYSLOG("idtr.base:  0x%x\n", idtr.base);
    SYSLOG("idtr.limit: %u\n", idtr.limit);

    for (uint8_t i=0; i<NUM_INTERRUPTS; i++)
    {
        usr_table[i] = nullptr;
    }

    for (uint8_t i=0; i<NUM_INTERRUPTS; i++)
    {
        idt_set_descriptor(i, __isr_table[i], INTERRUPT_GATE);
    }

    asm volatile ("lidt %0" : : "m"(idtr)); // Load IDT

    SYSLOG_END
}

