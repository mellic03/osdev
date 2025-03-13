// #include "./interrupt.hpp"
// #include "../gdt/gdt.hpp"
// #include "../drivers/serial.hpp"

// using namespace idk;
// using isr_type     = interrupt::isr_type;

// static constexpr size_t NUM_INTERRUPTS = 128;
// static idt_entry_t  idt[256] __attribute__((aligned(0x10))) ; 
// static idtr_t       idtr;
// static isr_type     isr_user_table[NUM_INTERRUPTS];


// extern "C"
// {
//     extern isr_type isr_stub_table[NUM_INTERRUPTS];

//     void dummy_callback( void )
//     {
//         serial_printf("[dummy_callback]\n");
//     }

//     __attribute__((noreturn))
//     void exception_handler( uint32_t code )
//     {
//         serial_printf("[exception_handler] code=%d\n", code);
//         isr_user_table[code]();

//         while (1)
//         {
            
//         }
//     }
// }



// void
// interrupt::registerSystemISR( uint8_t idx, isr_type callback )
// {
//     isr_user_table[idx] = callback;
// }



// void idt_set_descriptor( uint8_t idx, isr_type isr, uint8_t flags )
// {
//     idt_entry_t* descriptor = &idt[idx];

//     descriptor->isr_low    = (uint64_t)isr & 0xFFFF;
//     descriptor->kernel_cs  = GDT_OFFSET_KERNEL_CODE;
//     descriptor->ist        = 0;
//     descriptor->attributes = flags;
//     descriptor->isr_mid    = ((uint64_t)isr >> 16) & 0xFFFF;
//     descriptor->isr_high   = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
//     descriptor->reserved   = 0;
// }



// void interrupt::internal::init_IDT()
// {
//     idtr.base = (uintptr_t)&idt[0];
//     idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUM_INTERRUPTS - 1;

//     for (uint8_t i=0; i<NUM_INTERRUPTS; i++)
//     {
//         isr_user_table[i] = dummy_callback;
//     }
// }


// void interrupt::internal::load_IDT()
// {
//     for (uint8_t i=0; i<NUM_INTERRUPTS; i++)
//     {
//         idt_set_descriptor(i, isr_stub_table[i], 0x8E);
//     }

//     asm volatile ("lidt %0" : : "m"(idtr)); // Load IDT
//     asm volatile ("sti");                   // Enable interrupts
// }







#include "./interrupt.hpp"
#include "../gdt/gdt.hpp"
#include "../drivers/serial.hpp"

using namespace idk;
using isr_type = interrupt::isr_type;

inline static constexpr size_t NUM_INTERRUPTS = 128;
inline static idt_entry_t idt[256] __attribute__((aligned(0x10))) ; 
inline static idtr_t      idtr;
inline static isr_type    isr_table[NUM_INTERRUPTS];


template <uint8_t code>
__attribute__((noreturn))
void __isr_callback()
{
    serial_printf("[__isr_callback] code: %d\n", code);
    // asm volatile ("hlt");
    asm volatile ("iretq");

    while (1)
    {

    }
}


template <uint8_t idx>
void __register_isr_callback()
{
    isr_table[idx] = __isr_callback<idx>;
    __register_isr_callback<idx - 1>();
}


template <>
void __register_isr_callback<0>()
{
    isr_table[0] = __isr_callback<0>;
}



void
interrupt::registerSystemISR( uint8_t code, isr_type callback )
{
    isr_table[code] = callback;
}




void idt_set_descriptor( uint8_t idx, isr_type isr, uint8_t flags )
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



void interrupt::initIDT()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * NUM_INTERRUPTS - 1;
    __register_isr_callback<uint8_t(127)>();
}


void interrupt::loadIDT()
{
    for (uint8_t i=0; i<NUM_INTERRUPTS; i++)
    {
        idt_set_descriptor(i, isr_table[i], 0x8E);
    }

	asm volatile ("cli");                   // Disable interrupts
    asm volatile ("lidt %0" : : "m"(idtr)); // Load IDT
    asm volatile ("sti");                   // Enable interrupts
}

