#include "video.hpp"
#include "log.hpp"
#include "vterminal.hpp"
#include "system/drivers/serial.h"
#include "system/file/bmp.hpp"
#include "system/memory/memory.hpp"
#include <string.h>
#include <algorithm.hpp>



void
idk::Video::init( idk::linear_allocator &mem, limine_framebuffer *fb )
{
    SYSLOG_BEGIN("idk::Video::init");

    m_W = fb->width;
    m_H = fb->height;
    m_frontbuffer = { m_W, (uint32_t*)(fb->address) };
    m_backbuffer  = buf_wrapper<uint32_t>(m_W, mem.alloc<uint32_t>(m_W*m_H));
    m_depthbuffer = buf_wrapper<float>(m_W, mem.alloc<float>(m_W*m_H));

    m_winframes.init(64, mem.alloc<WinFrameBase*>(64));
    m_fonts.init(16, mem.alloc<FontBuffer>(16));

    SYSLOG("framebuffer: 0x%x\n", m_frontbuffer);
    SYSLOG("resolution:  %ux%ux%u\n", m_W, m_H, fb->bpp);

    SYSLOG_END
}


void
idk::Video::update()
{
    // bool dirty = false;

    for (auto *W: m_winframes)
    {
        W->render(*this);
        // if (win->dirty)
        // {
            // win->dirty = false;
            // renderWindow(win, nullptr);
            // dirty = true;
        // }
    }

    // if (dirty)
    // {
        swapBuffers();
    // }
}



void
idk::Video::swapBuffers()
{
    size_t count = m_W * m_H;

    memcpy32(m_frontbuffer[0], m_backbuffer[0], count);
    memset(m_depthbuffer[0], 127, count);
    memset32(m_backbuffer[0], 0, count);
}



void
idk::Video::rect( ivec2 cnr, ivec2 spn, const uvec4 &C, float z )
{
    uint32_t color = (C.b << 24) + (C.g << 16) + (C.r << 8) + (C.a);

    int xmin = std::max(cnr.x, 0);
    int xmax = std::min(cnr.x+spn.x, m_W-1);
    int ymin = std::max(cnr.y, 0);
    int ymax = std::min(cnr.y+spn.y, m_H-1);

    for (int y=ymin; y<ymax; y++)
    {
        for (int x=xmin; x<xmax; x++)
        {
            if (z < m_depthbuffer[y][x])
            {
                m_backbuffer[y][x] = color;
                m_depthbuffer[y][x] = z;
            }
        }
    }
}


void
idk::Video::blit( ivec2 dst, ivec2 src, ivec2 sp, const buf_wrapper<uint32_t> &buf, float z )
{
    int xmin = std::max(dst.x, 0);
    int xmax = std::min(dst.x+sp.x, m_W-1);
    int ymin = std::max(dst.y, 0);
    int ymax = std::min(dst.y+sp.y, m_H-1);

    for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
    {
        for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
        {
            if (z < m_depthbuffer[y][x])
            {
                m_backbuffer[y][x] = buf[sy][sx];
                m_depthbuffer[y][x] = z;
            }
        }
    }
}



void
idk::Video::loadFonts( limine_module_response *res )
{
    SYSLOG_BEGIN("idk::Video::loadFonts");
    SYSLOG("module_count=%d\n", res->module_count);

    for (size_t i=0; i<res->module_count; i++)
    {
        auto *file = res->modules[i];

        if (strncmp(file->string, "font", 4) == 0)
        {
            SYSLOG("font: \"%s\"\n", file->string);
            auto *header = (ck_BMP_header*)file->address;
            m_fonts.push_back(FontBuffer(file->string, header));
        }
    }

    SYSLOG_END
}



// idk::video::FONTS&
// idk::video::getFonts()
// {
//     return __video_fonts;
// }

