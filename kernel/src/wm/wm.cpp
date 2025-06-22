#include <wm/wm.hpp>
#include <wm/button.hpp>
#include <wm/mouse.hpp>
#include <wm/render.hpp>
#include <wm/text.hpp>
#include <wm/window.hpp>

#include <arch/io.hpp>
#include <sys/process.hpp>
#include <sys/tty0.hpp>

#include <driver/video.hpp>
#include <kernel/kvideo.hpp>
#include <kernel/event.hpp>
#include <kernel/log.hpp>
#include <kprintf.hpp>
#include <kpanic.hpp>
#include <kthread.hpp>
#include <kmalloc.h>

#include <filesystem/initrd.hpp>
#include <filesystem/vfs.hpp>
#include <bmp.hpp>
#include <new>


static void wm_mouseInput();
static void wm_keyInput();

static wm::guiRoot  wmRoot;
static wm::guiMouse wmMouse;
static wm::guiTextArea *wmTextArea;


void wm::main( void* )
{
    guiImage cursorimg(initrd::fopen("usr/share/img/cursor.bmp"));
    guiFont  txtfont(initrd::fopen("usr/share/font/cutive-w12hf18.bmp"));

    syslog::println("[wm::main] A");
    wmRoot.addChild(new guiButton("roo", ivec2(50), ivec2(100, 50), []() { syslog::println("Roo"); }));
    // wmTextArea = new guiTextTTY(&knl::tty0, &txtfont, ivec2(0, 200), ivec2(600, 600));
    wmTextArea = new guiTextArea(&txtfont, ivec2(0, 200), ivec2(600, 600));
    knl::tty0_stdout.listen(wmTextArea);
    syslog::println("[wm::main] B");

    auto win1 = new guiWindow(ivec2(50, 50), ivec2(450, 450));
    {
        wmTextArea->m_style.fillBounds = true;
        win1->addChild(wmTextArea);
    }
    wmRoot.addChild(win1);
    syslog::println("[wm::main] C");

    auto win0 = new guiWindow(ivec2(200, 100), ivec2(450, 450));
    {
        win0->m_style.solidColor = false;
        auto *txt = new guiTextInput(&txtfont, ivec2(0), ivec2(300, 50));
        txt->m_style.fillX = true;
        txt->onSubmit = []( guiTextInput *E ) {
            wmTextArea->putstr(E->m_buf);
            wmTextArea->putch('\n');
        };
        win0->addChild(txt);

        win0->addChild(new guiButton("256*256", ivec2(0, 75), ivec2(100, 50), []() {
            kvideo2::setMode(256, 256, 32);
        }));

        win0->addChild(new guiButton("B", ivec2(0, 150), ivec2(100, 50), []() {
            kprintf("[ButtonCallback]\n");
            kprintf("{\n");
            kprintf("    Test: %d\n", 12345);
            kprintf("}\n");
        }));

        win0->addChild(new guiButton("C", ivec2(0, 225), ivec2(100, 50), []() {
            kpanic("test");
        }));
    }
    wmRoot.addChild(win0);
    syslog::println("[wm::main] D");


    while (true)
    {
        auto dstimg = guiFramebuffer(
            kvideo2::fbmem, kvideo2::W, kvideo2::H,
            kvideo2::BYPP, kvideo2::Pitch
        );

        wmRoot.update(ivec2(0, 0), ivec2(kvideo2::W, kvideo2::H));
        wm_mouseInput();
        wm_keyInput();

        wmRoot.draw(dstimg);
        guiBlitImage(dstimg, cursorimg, wmMouse.xy);

        kvideo2::flush();
    }

}



extern std::mutex   mslock;
extern knl::MsState mscurr;

static void wm_mouseInput()
{
    knl::MsState msdata;

    {
        std::lock_guard lock(mslock);
        msdata = mscurr;
    }

    wmMouse.update(msdata, &wmRoot);
}


extern bool kbDevGetEvent( knl::KbEvent* );
static void wm_keyInput()
{
    knl::KbEvent kbdata;

    while (kbDevGetEvent(&kbdata))
    {
        if (wmMouse.focused)
        {
            wmMouse.focused->onKey(kbdata);
        }
    }

    // knl::KbEvent buf[8];
    // size_t nbytes = vfs::read(kbevent, buf, 0, sizeof(buf));
    // size_t count  = nbytes / sizeof(knl::KbEvent);

    // for (size_t i=0; i<count; i++)
    // {
    //     if (wmMouse.focused)
    //     {
    //         wmMouse.focused->onKey(buf[i]);
    //     }
    // }
}



// static void waitForVBlank()
// {
//     // Poll until not in vblank
//     while (IO::in8(0x3DA) & 0x08)
//         asm volatile ("nop");

//     // Poll until in vblank
//     while (!(IO::in8(0x3DA) & 0x08))
//         asm volatile ("nop");
// }




