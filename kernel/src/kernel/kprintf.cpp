#include <kprintf.hpp>
#include <kernel/event.hpp>
#include <kernel/log.hpp>
#include <kmemxx.hpp>

#include <ipc/pipe.hpp>
#include <filesystem/vfs.hpp>

#include <stdio.h>

// static guiButton button("Ree", {200, 200}, {200, 50});

static vfsNode *fh_stdout;
// static char     buf[1024];
// // static knlTTY *kTTY = nullptr;
// static guiTextArea *textright = nullptr;
// // static guiTextArea *textarea = nullptr;
// static guiFont font;
// static ivec2   cursor = ivec2(0, 0);



void kprintf_init()
{
    fh_stdout = knl::popen("/dev/stdout", 1);
    // syslog log("kprintf_init");
    // BMP_File bmp(initrd::fopen("usr/share/font/cutive-w14hf20.bmp"));
    // font = guiFont(bmp.pixels, bmp.w, bmp.h, bmp.bpp);
    // guiSetFont(&font);

    // textright = new guiTextArea(ivec2(kvideo::W/2, 0), ivec2(kvideo::W/2, kvideo::H/2));
    // textright->onClick = []() { textright->vscroll++; };

    // textarea = area;
}


void kprintf( const char *, ... )
{
    // va_list args;
    // va_start(args, fmt);
    // int n = vsprintf(buf, fmt, args);
    // va_end(args);

    // vfs::write(fh_stdout, buf, 0, (size_t)n);
    // textright->putstr(buf);
}

