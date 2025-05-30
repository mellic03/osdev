#include <wm/wm.hpp>
#include <wm/frame.hpp>
#include <wm/render.hpp>
#include <wm/text.hpp>
#include <wm/mouse.hpp>

#include <arch/io.hpp>
#include <kernel/kvideo.hpp>
#include <kernel/event.hpp>
#include <kernel/log.hpp>
#include <kthread.hpp>
#include <kmalloc.h>

#include <filesystem/initrd.hpp>
#include <filesystem/vfs.hpp>
#include <bmp.hpp>
#include <new>


static vfsNode *msstate;
static vfsNode *kbevent;
static vfsNode *fh_stdout;

static void wm_mouseInput();
static void wm_keyInput();
static void waitForVBlank();

static wm::guiRoot     wmRoot(ivec2(0, 0), ivec2(500, 500));
static wm::guiButton   wmButton("roo", ivec2(25, 25), ivec2(100, 50), []() { syslog::println("Roo"); });
static wm::guiMouse    wmMouse;
static wm::guiElement *wmFocus = nullptr;

void wm::main( void* )
{
    msstate = vfs::open("/dev/msstate");
    kbevent = vfs::open("/dev/kbevent");
    fh_stdout = vfs::open("/dev/stdout");

    guiFramebuffer framebuffer(kvideo::backbuffer, kvideo::W, kvideo::H, kvideo::BPP);
    guiImage       cursorimg(initrd::fopen("usr/share/img/cursor.bmp"));
    guiFont        txtfont(initrd::fopen("usr/share/font/cutive-w12hf18.bmp"));


    wmRoot.addChild(&wmButton);

    syslog::println("[wm::main] A");
    auto *ptr = new guiTextArea2(&txtfont, ivec2(0, 200), ivec2(600, 600));
    syslog::println("[wm::main] ptr=0x%lx", ptr);
    wmRoot.addChild(ptr);
    // wmRoot.addChild(new guiTextInput(&txtfont, ivec2(50), ivec2(300, 50)));
    // wmRoot.addChild(new guiWindow(ivec2(700, 100), ivec2(450, 450)));
    // wmRoot.addChild(new guiButton("roo", ivec2(25, 25), ivec2(100, 50), []() {
    //     syslog::println("Roo");
    // }));

    // wmRoot.m_children.push_back(wmTerm);
    // wmRoot.m_children.push_back(win);

    while (true)
    {
        wm_mouseInput();
        wm_keyInput();

        // char buf[64];
        // size_t nbytes = vfs::read(fh_stdout, buf, 0, sizeof(buf));

        // while (nbytes > 0)
        // {
        //     for (size_t i=0; i<nbytes; i++)
        //         wmTerm->putch(buf[i]);
        //     nbytes = vfs::read(fh_stdout, buf, 0, sizeof(buf));
        // }

        wmRoot.draw(framebuffer);
        guiBlitImage(framebuffer, cursorimg, wmMouse.xy);

        waitForVBlank();

        kvideo::swapBuffers();
        kthread::yield();
    }

}




// static void wm_findFocus()
// {
//     for (int i=int(wmRoot.m_children.size())-1; i>=0; i--)
//     {
//         auto *E = wmRoot.m_children[i];
    
//         if (E->isOver(mscurr.pos()))
//         {
//             wmFocus = E;
//             return;
//         }
//     }

//     wmFocus = nullptr;
// }


#include <kernel/ringbuffer.hpp>

static void wm_mouseInput()
{
    // static bool dragging = false;
    // static ivec2 startms  = ivec2(0, 0);
    // static ivec2 starttx  = ivec2(0, 0);

    static knl::MsState msprev, mscurr;
    
    if (vfs::read(msstate, &mscurr, 0, sizeof(mscurr)) == sizeof(mscurr))
    {
        wmMouse.delta = mscurr.pos() - msprev.pos();
        wmMouse.xy    = mscurr.pos();

        if (wmMouse.delta != ivec2(0, 0))
        {
            wmMouse.updateHovered(&wmRoot);
            if (wmMouse.focused)
                wmMouse.focused->onMouseMove(wmMouse);
        }

        if (mscurr.l && !msprev.l) // left down
        {
            wmRoot.onClick(wmMouse);
            wmFocus = wmMouse.focused;
        }

        if (!mscurr.l && msprev.l) // left up
        {
            wmMouse.dragging = false; 
        }

        if (wmMouse.dragging)
        {
            // ivec2 delta = mscurr.pos() - startms;
            // wmFocus->m_tl = starttx + delta;
        }
    
        msprev = mscurr;
    }
}


static void wm_keyInput()
{
    knl::KbEvent buf[8];

    size_t nbytes = vfs::read(kbevent, buf, 0, sizeof(buf));
    size_t count  = nbytes / sizeof(knl::KbEvent);

    for (size_t i=0; i<count; i++)
    {
        if (wmFocus)
        {
            wmFocus->onKey(buf[i]);
        }
    }
}



static void waitForVBlank()
{
    // Poll until not in vblank
    while (IO::in8(0x3DA) & 0x08)
        asm volatile ("nop");

    // Poll until in vblank
    while (!(IO::in8(0x3DA) & 0x08))
        asm volatile ("nop");
}





