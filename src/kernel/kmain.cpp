#include <kernel/terminal.h>
#include "../common/memory/memory.h"
#include "../common/memory/memory.hpp"
#include "../common/memory/linear_allocator.hpp"
#include "../common/terminal/terminal.h"
#include <libc/stdio.h>

#include "../boot/boot.hpp"
#include "../common/module.hpp"


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
    void kmain( uint32_t magic, uint32_t addr );
}




// static void
// process_tags( multiboot_tag *tag )
// {
//     // ckModulesInit(tag);

//     // while (tag->type != MULTIBOOT_TAG_TYPE_END)
//     // {
//     //     process_tag(tag);
//     //     tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
//     // }

// }


void kmain( uint32_t magic, uint32_t addr ) 
{
    // ckTerminal_t term = {
    //     .VGA_WIDTH  = 80,
    //     .VGA_HEIGHT = 25,
    //     .row = 0,
    //     .col = 0,
    //     .color = 0,
    //     .buf = (uint16_t*)(0xB8000)
    // };

    // ckTerminalInit(&term);

    // printf("[kmain]\n");
}