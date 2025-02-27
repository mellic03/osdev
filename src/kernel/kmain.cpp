#include <kernel/terminal.h>
#include "memory/memory.h"
#include "memory/memory.hpp"
#include "memory/linear_allocator.hpp"
#include "terminal/terminal.h"
#include <libc/stdio.h>

#include "../boot/boot.hpp"
#include "module.hpp"


// __attribute__((used, section(".limine_requests")))
// static volatile LIMINE_BASE_REVISION(3);

// __attribute__((used, section(".limine_requests")))
// static volatile struct limine_framebuffer_request framebuffer_request = {
//     .id = LIMINE_FRAMEBUFFER_REQUEST,
//     .revision = 0
// };





// __attribute__((used, section(".multiboot_header")))
// static volatile multiboot_;

// __attribute__((used, section(".multiboot_header_end")))
// static volatile LIMINE_REQUESTS_END_MARKER;


extern "C" {
    void kmain();
    // void kmain( uint32_t magic, uint32_t addr );
}




static void
process_tags( multiboot_tag *tag )
{
    ckModulesInit(tag);

    // while (tag->type != MULTIBOOT_TAG_TYPE_END)
    // {
    //     process_tag(tag);
    //     tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
    // }

}


void kmain() 
// void kmain( uint32_t magic, uint32_t addr ) 
{
    ckTerminal_t term = {
        .VGA_WIDTH  = 80,
        .VGA_HEIGHT = 25,
        .row = 0,
        .col = 0,
        .color = 0,
        .buf = (uint16_t*)(0xB8000)
    };

    ckTerminalInit(&term);

    // if (ck::kboot(magic, addr) == -1)
    // {
    //     return;
    // }


    // process_tags((multiboot_tag*)(addr+8));

    // // ck::linear_allocator *mainblock;
    // ckMemoryInfo memory = {
    //     ck::linear_allocator(),
    //     ck::stack_allocator()
    // };

    // if (ckMemoryInit(addr, &memory) == -1)
    // {
    //     printf("Error initializing memory\n");
    //     return;
    // }

    printf("[kmain]\n");
}