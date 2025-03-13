#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
// #include <initializer_list>

#include "bootinfo.hpp"
#include "drivers/serial.h"
#include "drivers/serial.hpp"
#include "io.hpp"
#include "memory/memory.hpp"
#include "video/video.hpp"

#include <stdio.h>
#include <cstdio.hpp>
#include <cstring.hpp>

#include "system/system.hpp"


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

static void hcf()
{
    while (true)
    {
        asm("hlt");
    } 
}




void kmain( const ckBootInfo &info )
{
    auto *fb = info.fb_list[0];
    fb_init(*fb);
    fb_rect({0, 0, 100, 50}, {0, 255, 0, 255});
    
    
    idk::linear_allocator mainblock;
    {
        uint64_t base = 0x00;
        uint64_t len  = 0;

        for (uint64_t i=0; i<info.mmap->entry_count; i++)
        {
            auto &entry = info.mmap->entries[i];

            if (entry->type == LIMINE_MEMMAP_USABLE)
            {
                if (entry->length > len)
                {
                    len = entry->length;
                    base = entry->base;
                }

                // ck::serial::writeln(": 0x%x\n", entry->base);
                // serial_printf("memory: 0x%x\n", entry->base);
            }
        }
    
        mainblock = idk::linear_allocator(len, (void*)(0xFFFF800000000000 + base));
    }

    if (!idk::memory::init(&mainblock))
    {
        return;
    }

    if (!ck::serial::init(&mainblock))
    {
        return;
    }
    
    if (!idk::sys::init(&mainblock))
    {
        return;
    }


    auto *buddy = idk::memory::getBuddy();
    void *deez = buddy->alloc(256*sizeof(uint8_t), 1);
    // void *nuts = buddy->alloc(54*sizeof(uint8_t), 1);
    // serial_printf("deez: 0x%x\n", deez);
    // serial_printf("nuts: 0x%x\n", nuts);
    buddy->free(deez);

    idk::SysRequest req(
        idk::SysRequestType::FILE_CREATE,
        "TestLMAO"
    );


    auto &res = idk::syscall(req);
    serial_printf("res.type: %d\n", res.type);

    hcf();
}


