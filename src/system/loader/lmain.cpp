#include <kcommon/memory.hpp>
#include <kcommon/terminal.hpp>
#include <kcommon/module.hpp>
#include <kcommon/multiboot2.hpp>
#include <kcommon/stack.hpp>


#include <cstdio.hpp>
#include <cstring.hpp>

#include "./gdt-32.hpp"
#include "./drivers/serial.hpp"
#include "./drivers/ps2.hpp"
#include "../interpreter/interpreter.hpp"

#include "./info.hpp"
#include "../kcommon/file/bmp.hpp"



// extern char __multiboot_start;
// extern char __multiboot_end;


extern "C" {
    void lmain( uint32_t magic, uint32_t *addr );
    void _enter_kernel( void *entry, uint32_t multiboot_info );
}


void lmain( uint32_t magic, uint32_t *addr ) 
{
    // uint16_t term_buffer[25*80];

    ckTerminal term(
        80, 25, (uint16_t*)(0xB8000),
        80, 25, (uint16_t*)(0xB8000)
        // 80, 25, term_buffer
    );

    ckTerminalInit(term);


    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        return;
    }
    std::printf("magic == MULTIBOOT2_BOOTLOADER_MAGIC\n");


    // if (!ck::serial::init())
    // {
    //     // std::printf("Failed to initialize serial\n");
    //     return;
    // }
    // std::printf("Initialized serial\n");
    term.flush();


    ckInfo info = ckInfoLoad(addr+16);


    if (info.font != 0)
    {
        ck_BMP_header *bmp = reinterpret_cast<ck_BMP_header*>(info.font);
    
        std::printf(
            "address=%u, signature=%u, filesize=%u, reserved=%u, offset=%u\n",
            info.font, bmp->signature, bmp->filesize, bmp->reserved, bmp->offset
        );
    }



    std::printf("test value:   0x%x\n", info.memory.stack.test_value);
    std::printf("total nbytes: 0x%x\n", info.memory.total_size);
    


    auto &mem = info.memory;

    std::printf("\n\num_mmaps: %d\n", mem.num_mmaps);

    for (uint32_t i=0; i<mem.num_mmaps; i++)
    {
        std::printf("\t0x%x  %u bytes\n", mem.mmaps[i], mem.mmaps[i]);
    }



    // ck_gdt_table GDT;
    // ck_TSS TSS;
    // ckGDT32_create(&GDT, &TSS);
    // ckGDT32_load(&GDT, &TSS);
    // std::printf("[lmain] gdt initalized\n");
    // term.flush();


    // uint8_t lo, hi;

    // ck::IO::outb(0x70, 0x30);
    // lo = ck::IO::inb(0x71);
    // ck::IO::outb(0x70, 0x31);
    // hi = ck::IO::inb(0x71);

    // uint32_t CMOS = (uint32_t(hi) << 8) + uint32_t(lo);

    term.flush();

}







// void load_font( multiboot_tag *mb_tag )
// {
//     auto *tag = mb_tag;

//     while (tag->type != MULTIBOOT_TAG_TYPE_END)
//     {
//         if (tag->type == MULTIBOOT_TAG_TYPE_MODULE)
//         {
//             auto *mod = (multiboot_tag_module*)(tag);
        
//             if (std::strcmp(mod->cmdline, "font-test") == 0)
//             {
//                 _load_font(mod);
//             }
//         }

//         tag = (multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
//     }
// }
