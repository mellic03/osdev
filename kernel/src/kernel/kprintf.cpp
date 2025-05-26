#include <kprintf.hpp>
#include <kmemxx.hpp>
#include <kernel/event.hpp>
#include <kernel/log.hpp>

#include <stdio.h>
#include <kernel/module.hpp>
#include <kernel/kvideo.hpp>
#include <kernel/tty.hpp>
#include <filesystem/initrd.hpp>

#include <gui/gui.hpp>
#include <cringe/bmp.hpp>
#include <cringe/vec.hpp>

static guiButton button("Ree", {200, 200}, {200, 50});

static knlTTY *kTTY = nullptr;
static guiTextArea *textarea = nullptr;
static guiFont font;
static ivec2   cursor = ivec2(0, 0);
static char    buf[1024];

void kprintf_redraw( void* )
{
    guiFramebuffer fb(kvideo::backbuffer, kvideo::W, kvideo::H, kvideo::BPP);
    auto *msdev = (CharDevInterface*)knl::findModule(ModuleType_Device, DeviceType_Mouse);

    guiContainer bleft(0, 0, kvideo::W/2, kvideo::H);
    guiContainer bright(kvideo::W/2, 0, kvideo::W, kvideo::H);


    button.draw(&fb);

    ivec2 csr = cursor;
    textarea->draw(&fb);

    csr = cursor;
    csr = guiRenderFont(&fb, bright, kTTY->buf, csr);
    csr = guiRenderFont(&fb, bright, "_", csr);

    knl::MsState msdata;
    msdev->read(&msdata, sizeof(msdata));
    guiRenderHLine(&fb, ivec4(255, 55, 55, 255), 0, fb.w, msdata.y);
    guiRenderVLine(&fb, ivec4(55, 55, 255, 255), msdata.x, 0, fb.h);
}


void kprintf_init( knlTTY *tty, guiTextArea *area )
{
    syslog log("kprintf_init");
    BMP_File bmp(initrd::fopen("usr/share/font/cutive-w12hf18.bmp"));
    font = guiFont((uint8_t*)(bmp.data), bmp.w, bmp.h, bmp.bpp);
    guiSetFont(&font);

    kTTY = tty;
    textarea = area;
}


void kprintf( const char *fmt, ... )
{
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    kTTY->putstr(buf);
}

