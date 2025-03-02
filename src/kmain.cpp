#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "boot/bootinfo.hpp"
#include "drivers/serial.hpp"
#include "io.hpp"
#include "kcommon/ctor/icxxabi.h"

#include <stdio.h>
#include <cstdio.hpp>
#include <cstring.hpp>

#include "./ck_systemio/ck_systemio.h"

#include "gdt/gdt.hpp"
#include "interrupt/interrupt.h"


// /* Check if RTC is updating */
// static int is_updating()
// {
//     ck::IO::outb(0x70, 0x0A);
//     return ck::IO::inb(0x71) & 0x80;
// }

// static unsigned char read(int reg)
// {
//     while (is_updating());
//     ck::IO::outb(0x70, reg);
//     return ck::IO::inb(0x71);
// }


void kmain( ckBootInfo *info )
{
    if (!info)
    {
        for (int i=0; 1000; i++)
        {
            ck::serial::writeln("!info\n");
        }
    }

    if (info && info->fb_count == 1)
    {
        
    }

    if (!ck::serial::init())
    {

    }

    ck::serial::writeln("Serial initialized.\n");

    if (!ckSystemIO_init())
    {
        
    }
    ck::serial::writeln("ckSystemIO initialized.\n");


    asm("cli");
    auto GDTR = init_GDT();
    printf("[reload_gdt]\n");

    // asm("sti");
    // init_IDT();
    // printf("[init_IDT]\n");


    if (info)
    {
        for (auto i=0; i<info->mmap->entry_count; i++)
        {
            auto &entry = info->mmap->entries[i];

            if (entry->type == LIMINE_MEMMAP_USABLE)
            {
                // ck::serial::writeln(": 0x%x\n", entry->base);
                fprintf(stdout, "memory: 0x%x\n", entry->base);
            }
        }
    }


    while (true)
    {
        asm("hlt");
    }
    hcf();
}




// struct idt_entry {
//     uint16_t base_lo;
//     uint16_t sel;
//     uint8_t always0;
//     uint8_t flags;
//     uint16_t base_hi;
// } __attribute__((packed));

// struct idt_ptr {
//     uint16_t limit;
//     uint32_t base;
// } __attribute__((packed));

// extern void isr0(); // ISR for interrupt 0
// extern void isr1(); // ISR for interrupt 1
// // Declare other ISRs as needed

// struct idt_entry idt[256];  // IDT for 256 possible interrupts
// struct idt_ptr idt_ptr;

// void set_idt_entry(int n, uint64_t base, uint16_t sel, uint8_t flags) {
//     idt[n].base_lo = (uint16_t)(base & 0xFFFF);
//     idt[n].base_hi = (uint16_t)((base >> 16) & 0xFFFF);
//     idt[n].sel = sel;
//     idt[n].always0 = 0;
//     idt[n].flags = flags;
// }

// void idt_load()
// {
//     idt_ptr.limit = sizeof(idt) - 1;
//     idt_ptr.base = reinterpret_cast<uint32_t>(&idt);
//     __asm__ __volatile__("lidt %0" : : "m"(idt_ptr));
// }

// void isr0()
// {
//     // Handle interrupt 0 (e.g., Divide-by-zero error)
//     // You may want to print an error message, or panic here
// }

// void isr1()
// {
//     // Handle interrupt 1 (e.g., Single-step exception)
// }




// void init_idt()
// {
//     // Set up ISRs
//     set_idt_entry(0, (uint64_t)isr0, 0x08, 0x8E);  // Example ISR 0 setup
//     set_idt_entry(1, (uint64_t)isr1, 0x08, 0x8E);  // Example ISR 1 setup
//     // Add more entries as needed

//     // Load the IDT
//     idt_load();
// }
