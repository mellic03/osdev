#include "video.hpp"
#include "vterminal.hpp"
#include "system/drivers/serial.h"
#include "system/file/bmp.hpp"
#include "system/memory/memory.hpp"
#include <string.h>
#include <algorithm.hpp>


static limine_framebuffer *__video_fb;
static size_t    __video_w;
static size_t    __video_h;
static size_t    __video_npixels;
static size_t    __video_nbytes;
static uint32_t *__video_frontbuffer;
static uint32_t *__video_backbuffer;
static idk::video::FONTS __video_fonts;


int
idk::video::init( idk::linear_allocator &mem, limine_framebuffer *fb )
{
    serial_printf("[idk::video::init]\n");

    __video_fb          = fb;
    __video_w           = fb->width;
    __video_h           = fb->height;
    __video_npixels     = __video_w * __video_h;
    __video_nbytes      = __video_npixels * sizeof(uint32_t);
    __video_frontbuffer = (uint32_t*)(__video_fb->address);
    __video_backbuffer  = (uint32_t*)(mem.alloc(__video_nbytes, alignof(uint32_t)));

    __video_fonts.count = 0;
    __video_fonts.cap   = 32;
    __video_fonts.fonts = (FontBuffer*)mem.alloc(32*sizeof(FontBuffer), alignof(FontBuffer));

    return 1;
}


void
idk::video::swapBuffers()
{
    size_t count = __video_fb->width * __video_fb->height;

    memcpy32(__video_frontbuffer, __video_backbuffer, count);
    memset32(__video_backbuffer, 0, count);
}










void
idk::video::renderRect( int x0, int y0, int w, int h, const uvec4 &C )
{
    uint32_t  color = (C.r << 24) + (C.g << 16) + (C.b << 8) + (C.a);
    uint32_t *dst   = __video_backbuffer;
    const int W     = __video_w;
    const int H     = __video_h;

    int xmin = x0;
    int xmax = xmin + w;
    int ymin = y0;
    int ymax = ymin + h;

    xmin = std::clamp(xmin, 0, W);
    xmax = std::clamp(xmax, 0, W);
    ymin = std::clamp(ymin, 0, H);
    ymax = std::clamp(ymax, 0, H);

    for (int y=ymin; y<ymax; y++)
    {
        for (int x=xmin; x<xmax; x++)
        {
            dst[y*W + x] = color;
        }
    }
}





void
idk::video::renderTerminal( idk::VWindow *win, idk::Terminal *term, FontBuffer *font )
{
    video::blit(win->x, win->y, *font);
}




void
idk::video::renderWindow( idk::VWindow *win, idk::VWindow *parent )
{
    int x = win->x + ((parent) ? parent->x : 0);
    int y = win->y + ((parent) ? parent->y : 0);
    int w = win->w;
    int h = win->h;

    idk::video::renderRect(x, y, w, h, win->bg);

    if (win->term && win->font)
    {
        idk::video::renderTerminal(win, win->term, win->font);
    }

    for (size_t i=0; i<win->children.capacity(); i++)
    {
        if (win->children[i])
        {
            video::renderWindow(win->children[i], win);
        }
    }
}



void
idk::video::blit( int x0, int y0, FontBuffer &src )
{
    if (src.data() == nullptr)
    {
        return;
    }

    uint32_t *pixels = __video_backbuffer;
    const int W = __video_w;
    const int H = __video_h;

    int xmin = x0;
    int xmax = xmin + src.W;
    int ymin = y0;
    int ymax = ymin + src.H;

    xmin = std::clamp(xmin, 0, W);
    xmax = std::clamp(xmax, 0, W);
    ymin = std::clamp(ymin, 0, H);
    ymax = std::clamp(ymax, 0, H);

    for (int y=ymin; y<ymax; y++)
    {
        for (int x=xmin; x<xmax; x++)
        {
            int sx = std::clamp(x-x0, 0, src.W);
            int sy = std::clamp(y-y0, 0, src.H);
            pixels[y*W + x] = src[sy][sx];
        }
    }

}





int
idk::video::loadFonts( limine_module_response *res )
{
    serial_printf("\n");
    serial_printf("[idk::video::loadFonts] module_count=%d\n", res->module_count);

    for (size_t i=0; i<res->module_count; i++)
    {
        auto *file = res->modules[i];

        if (strncmp(file->string, "font", 4) == 0)
        {
            serial_printf("[idk::video::loadFonts] font: \"%s\"\n", file->string);

            auto *header = (ck_BMP_header*)file->address;

            size_t idx = __video_fonts.count++;
            auto *font = &(__video_fonts.fonts[idx]);
            font = new (font) FontBuffer(file->string, header);

            // font.init(file->string, header);
        }
    }

    // if (!file)
    // {
    //     return 0;
    // }

    // serial_printf("Loaded font \"%s\"\n", file->string);
    // auto *font = new video::FontBuffer((ck_BMP_header*)file->address);

    // serial_printf("num fonts: %d\n", __video_fonts.count);

    return 1;
}



idk::video::FONTS&
idk::video::getFonts()
{
    return __video_fonts;
}

