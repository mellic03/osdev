#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "system/system.hpp"
#include "system/interrupt/interrupt.hpp"
#include "system/drivers/kb-ps2.hpp"


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


    auto &video = system.video;
    video.loadFonts(info.modules);

    auto *win  = video.createWindow({0, 0}, {300, 500});
    win->term = system.createTerminal(25, 25);
    win->font = &(video.getFonts())[6];

    while (true)
    {
        char key = idk::PS2::kb_poll();

        if (!key)
        {
            continue;
        }

        if (key == 'w') { win->y -= 1; }
        if (key == 's') { win->y += 1; }
        if (key == 'a') { win->x -= 1; }
        if (key == 'd') { win->x += 1; }
    
        if (win->x >= 100)
        {
            win->x = 0;
            idk::Interrupt(idk::Exception::TEST_VALUE);
        }
    
        system.video.renderWindow(win, nullptr);
        system.video.swapBuffers();
    }


    hcf();
}


