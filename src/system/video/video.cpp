#include "video.hpp"
#include "vterminal.hpp"
#include "system/drivers/serial.h"
#include "system/file/bmp.hpp"
#include "system/memory/memory.hpp"
#include <string.h>
#include <algorithm.hpp>



int
idk::Video::init( idk::linear_allocator &mem, limine_framebuffer *fb )
{
    serial_printf("[idk::Video::init]\n");

    m_W = fb->width;
    m_H = fb->height;
    m_frontbuffer = (uint32_t*)(fb->address);
    m_backbuffer  = mem.alloc<uint32_t>(m_W * m_H);

    m_windows.init(64, mem.alloc<VWindow*>(64));
    m_fonts.init(16, mem.alloc<FontBuffer>(16));

    // __video_fb          = fb;
    // __video_w           = fb->width;
    // __video_h           = fb->height;
    // __video_npixels     = __video_w * __video_h;
    // __video_nbytes      = __video_npixels * sizeof(uint32_t);
    // __video_frontbuffer = (uint32_t*)(__video_fb->address);
    // __video_backbuffer  = (uint32_t*)(mem.alloc(__video_nbytes, alignof(uint32_t)));

    // __video_fonts.count = 0;
    // __video_fonts.cap   = 32;
    // __video_fonts.fonts = (FontBuffer*)mem.alloc(32*sizeof(FontBuffer), alignof(FontBuffer));

    return 1;
}


void
idk::Video::swapBuffers()
{
    size_t count = m_W * m_H;

    memcpy32(m_frontbuffer, m_backbuffer, count);
    memset32(m_backbuffer, 0, count);
}




idk::VWindow*
idk::Video::createWindow( const ivec2 &corner, const ivec2 &extents )
{
    static int id = 0;
    m_windows.push_back(new VWindow(id++, corner, extents, uvec4(155)));
    return m_windows.back();
}






void
idk::Video::renderRect( int x0, int y0, int w, int h, const uvec4 &C )
{
    uint32_t  color = (C.r << 24) + (C.g << 16) + (C.b << 8) + (C.a);
    uint32_t *dst   = m_backbuffer;
    const int W     = m_W;
    const int H     = m_H;

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
idk::Video::renderTerminal( idk::VWindow *win, idk::Terminal *term, FontBuffer *font )
{
    blit(win->x, win->y, font);
}




void
idk::Video::renderWindow( idk::VWindow *win, idk::VWindow *parent )
{
    int x = win->x + ((parent) ? parent->x : 0);
    int y = win->y + ((parent) ? parent->y : 0);
    int w = win->w;
    int h = win->h;

    renderRect(x, y, w, h, win->bg);

    if (win->term && win->font)
    {
        renderTerminal(win, win->term, win->font);
    }

    for (size_t i=0; i<4; i++)
    {
        if (win->children[i])
        {
            renderWindow(win->children[i], win);
        }
    }
}



void
idk::Video::blit( int x0, int y0, FontBuffer *src )
{
    uint32_t *dst = m_backbuffer;
    const int W = m_W;
    const int H = m_H;

    int xmin = x0;
    int xmax = xmin + src->W;
    int ymin = y0;
    int ymax = ymin + src->H;

    xmin = std::clamp(xmin, 0, W);
    xmax = std::clamp(xmax, 0, W);
    ymin = std::clamp(ymin, 0, H);
    ymax = std::clamp(ymax, 0, H);

    for (int y=ymin; y<ymax; y++)
    {
        for (int x=xmin; x<xmax; x++)
        {
            float alpha = float(uint8_t(dst[y*W + x])) / 255.0f;

            int sx = std::clamp(x-x0, 0, src->W);
            int sy = std::clamp(y-y0, 0, src->H);
            dst[y*W + x] = (*src)[sy][sx];
        }
    }
}





int
idk::Video::loadFonts( limine_module_response *res )
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
            m_fonts.push_back(FontBuffer(file->string, header));
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



// idk::video::FONTS&
// idk::video::getFonts()
// {
//     return __video_fonts;
// }

