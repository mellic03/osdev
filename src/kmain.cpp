#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "system/system.hpp"
#include "system/interrupt/interrupt.hpp"
#include "system/drivers/kb-ps2.hpp"


void hcf()
{
    while (true)
    {
        asm("hlt");
    } 
}



static idk::VWindow *win;
static idk::System  *sys;

// void video_main()
// {
//     SYSLOG("[video_main]\n");

//     while (true)
//     {
//         char key = idk::PS2::kb_poll();

//         if (!key)
//         {
//             continue;
//         }

//         if (key == 'w') { win->y -= 1; }
//         if (key == 's') { win->y += 1; }
//         if (key == 'a') { win->x -= 1; }
//         if (key == 'd') { win->x += 1; }
    
//         if (win->x >= 100)
//         {
//             win->x = 0;
//             asm volatile ("int $102");
//             asm volatile ("sti");
//             // idk::Interrupt(idk::Exception::TEST_VALUE);
//         }
    
//         sys->video.renderWindow(win, nullptr);
//         sys->video.swapBuffers();
//     }
// }




void kmain( const idk_BootInfo &info )
{
    using namespace idk;

    auto &system = idk::getSystem();
    system.init(info);
    sys = &system;

    auto &video = system.video;
    video.loadFonts(info.modules);

    win  = video.createWindow({0, 0}, {300, 500});
    win->term = system.createTerminal(25, 25);
    win->font = &(video.getFonts())[6];


    {

        uint32_t type  = SysRequest_FILE_CREATE;
        uint32_t flags = SysRequestFlag::NONE;
        char buf[256] = "test.txt";
     
        SYSLOG("\n[kmain] Attempting syscall...\n\n");
        idk::Syscall(&type, &flags, buf);
        SYSLOG("YEAH BOYEE, res.type=%u\n", type);
    }

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
            // win->x = 0;
            // asm volatile ("int $102");
            // asm volatile ("sti");
            // idk::Interrupt(idk::Exception::TEST_VALUE);
        }
    
        system.video.renderWindow(win, nullptr);
        system.video.swapBuffers();
    }

    hcf();
}


