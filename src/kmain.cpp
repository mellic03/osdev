#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "system/system.hpp"


static void hcf()
{
    while (true)
    {
        asm("hlt");
    } 
}



void kmain( const idk_BootInfo &info )
{
    auto &system = idk::getSystem();
    system.init(info);

    // if (info.mp)
    // {
    //     auto &mp = *info.mp;
    //     serial_printf("cpu_count: %d\n", mp.cpu_count);
    // }

    // if (info.dtb)
    // {
    //     fdt_header *header = (fdt_header*)(info.dtb->dtb_ptr);
    //     serial_printf("magic: 0x%x\n", header->magic);
    // }

    // else
    // {
    //     serial_printf("No dtb\n");
    // }


    // serial_printf("model: %d\n", cpu.getModel());
    // serial_printf("cpu.cr0: %lu\n", *(uint64_t*)(&cpu.cr0));

    // using namespace idk;

    // idk::video::loadFonts(info.modules);
    // auto &fonts = idk::video::getFonts();

    // idk::Terminal term0(32, 64, (char*)mainblock.alloc(32*64*sizeof(char), alignof(char)));
    // idk::VWindow win({100, 100}, {350, 500}, uvec4(200));
    // win.term = &term0;
    // win.font = &(fonts.fonts[0]);

    // win.children[0] = new idk::VWindow({10, 10}, {330, 200}, uvec4(150));
    // win.children[0]->term = nullptr;
    // win.children[0]->font = nullptr;

    // while (true)
    // {
    //     char key = idk::PS2::kb_poll();

    //     if (!key)
    //     {
    //         continue;
    //     }

    //     if (key == 'w') { win.y -= 1; }
    //     if (key == 's') { win.y += 1; }
    //     if (key == 'a') { win.x -= 1; }
    //     if (key == 'd') { win.x += 1; }
    
    //     if (win.x >= 100)
    //     {
    //         win.x = 0;
    //         idk::Interrupt(idk::Exception::TEST_VALUE);
    //     }
    
    //     idk::video::renderWindow(&win, nullptr);
    //     idk::video::swapBuffers();
    // }


    hcf();
}


