#include <wm/wm.hpp>
#include <wm/button.hpp>
#include <wm/mouse.hpp>
#include <wm/render.hpp>
#include <wm/text.hpp>
#include <wm/window.hpp>

#include <arch/io.hpp>
#include <sys/process.hpp>
#include <driver/video.hpp>
#include <kernel/kvideo.hpp>
#include <kernel/event.hpp>
#include <kernel/log.hpp>
#include <kpanic.hpp>
#include <kthread.hpp>
#include <kmalloc.h>

#include <filesystem/initrd.hpp>
#include <filesystem/vfs.hpp>
#include <bmp.hpp>
#include <new>


static vfsNode *msstate;
static vfsNode *kbevent;
// static vfsNode *fh_stdout;

static void wm_mouseInput();
static void wm_keyInput();
// static void waitForVBlank();

static wm::guiRoot  wmRoot;
static wm::guiMouse wmMouse;
static wm::guiTextArea *wmTextArea;



// static void thread0( void* )
// {
//     while (true)
//     {
//         syslog::println("[wm::thread0]");
//         knl::threadYield();
//     }
// }


// static void thread1( void* )
// {
//     while (true)
//     {
//         syslog::println("[wm::thread1]");
//         knl::threadYield();
//     }
// }




void wm::main( void* )
{
    // knl::createThread(nullptr, thread0, nullptr);
    // knl::createThread(nullptr, thread1, nullptr);
    msstate = vfs::open("/dev/msstate");
    kbevent = vfs::open("/dev/kbevent");
    // fh_stdout = vfs::open("/dev/stdout");

    guiImage cursorimg(initrd::fopen("usr/share/img/cursor.bmp"));
    guiFont  txtfont(initrd::fopen("usr/share/font/cutive-w12hf18.bmp"));

    // wmRoot.addChild(new guiTextArea2(&txtfont, ivec2(0, 200), ivec2(600, 600)));
    wmRoot.addChild(new guiButton("roo", ivec2(50), ivec2(100, 50), []() { syslog::println("Roo"); }));
    
    wmTextArea = new guiTextArea(&txtfont, ivec2(0, 200), ivec2(600, 600));
    auto win1 = new guiWindow(ivec2(50, 50), ivec2(450, 450));
    {
        wmTextArea->m_style.fillBounds = true;
        win1->addChild(wmTextArea);
    }
    wmRoot.addChild(win1);

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

        win0->addChild(new guiButton("A", ivec2(0, 75), ivec2(100, 50), []() {
            kvideo2::setMode(1280, 720, 32);
        }));

        win0->addChild(new guiButton("B", ivec2(0, 150), ivec2(100, 50), []() {
            kvideo2::setMode(800, 600, 32);
        }));

        win0->addChild(new guiButton("C", ivec2(0, 225), ivec2(100, 50), []() {
            kpanic("test");
        }));
    }
        //  win0->addChild(new guiButton("Yee", ivec2(25, 25), ivec2(100, 50), [](){ kpanic("Yee"); }));
        //  win0->addChild(new guiImage(initrd::fopen("usr/share/img/mountains.bmp")));
    wmRoot.addChild(win0);

    // vec3 rgb0(0.6, 0.6, 0.85);
    // vec3 rgb1(0.9, 0.2, 0.65);
    // vec3 rgb2 = vec_mix(rgb0, rgb1, 0.25f);
    // u8vec3 rgb = u8vec3(255.0f * rgb2);
    // syslog::println("rgb3: %u %u %u", rgb.r, rgb.g, rgb.b);

    while (true)
    {
        // auto *buf = (uint64_t*)0x1000;
        // syslog::println("[wm::main] *0x1000: %lu", buf[0]);
        // syslog::println("[wm::main]");

        auto dstimg = guiFramebuffer(
            kvideo2::fbmem, kvideo2::W, kvideo2::H,
            kvideo2::BYPP, kvideo2::Pitch
        );

        wmRoot.update(ivec2(0, 0), ivec2(kvideo2::W, kvideo2::H));
        wm_mouseInput();
        wm_keyInput();

        wmRoot.draw(dstimg);
        guiBlitImage(dstimg, cursorimg, wmMouse.xy);

        // waitForVBlank();

        kvideo2::flush();
    }

}



static void wm_mouseInput()
{
    static knl::MsState msdata;
    
    if (vfs::read(msstate, &msdata, 0, sizeof(msdata)) == sizeof(msdata))
    {
        wmMouse.update(msdata, &wmRoot);
    }
}


static void wm_keyInput()
{
    knl::KbEvent buf[8];
    size_t nbytes = vfs::read(kbevent, buf, 0, sizeof(buf));
    size_t count  = nbytes / sizeof(knl::KbEvent);

    for (size_t i=0; i<count; i++)
    {
        if (wmMouse.focused)
        {
            wmMouse.focused->onKey(buf[i]);
        }
    }
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




