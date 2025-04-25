// extern "C"
// {
//     #define LIMINE_API_REVISION 3
//     #include <limine/limine.h>
// }

// #include "video.hpp"
// #include <kernel/log.hpp>
// #include <kmalloc.h>
// #include <kmemxx.h>
// #include <string.h>
// #include <algorithm>


// using namespace idk;


// void
// Video::init( limine_framebuffer_response *res )
// {
//     syslog log("idk::Video::init");
//     auto *fb = res->framebuffers[0];

//     m_W = fb->width;
//     m_H = fb->height;
//     m_frontbuffer = { m_W, (uint32_t*)(fb->address) };

//     m_backbuffer  = buf_wrapper<uint32_t>(m_W, (uint32_t*)kmalloc(m_W*m_H*sizeof(uint32_t)));
//     // m_depthbuffer = buf_wrapper<float>(m_W, (float*)kmalloc(m_W*m_H*sizeof(float)));

//     m_winframes.init((WinFrameBase**)kmalloc(64*sizeof(WinFrameBase*)), 32);
//     m_fonts.init((FontBuffer*)kmalloc(16*sizeof(FontBuffer)), 16);

//     log("framebuffer: 0x%x", m_frontbuffer);
//     log("resolution:  %ux%ux%u", m_W, m_H, fb->bpp);
// }


// void
// idk::Video::update()
// {
//     // bool dirty = false;

//     for (auto *W: m_winframes)
//     {
//         W->render(*this);
//         // if (win->dirty)
//         // {
//             // win->dirty = false;
//             // renderWindow(win, nullptr);
//             // dirty = true;
//         // }
//     }

//     // if (dirty)
//     // {
//         swapBuffers();
//     // }
// }



// void
// idk::Video::swapBuffers()
// {
//     size_t count = m_W * m_H;

//     kmemcpy<uint32_t>(m_frontbuffer[0], m_backbuffer[0], count);
//     // memset(m_depthbuffer[0], 127, count);
//     kmemset<uint32_t>(m_backbuffer[0], 0, count);
// }



// void
// idk::Video::rect( ivec2 tl, ivec2 sp, const uvec4 &C, float z )
// {
//     uint32_t color = (C.r << 24) + (C.g << 16) + (C.b << 8) + (C.a);

//     // int xmin = std::max(tl.x, 0);
//     // int xmax = std::min(cnr.x+spn.x, m_W-1);
//     // int ymin = std::max(cnr.y, 0);
//     // int ymax = std::min(cnr.y+spn.y, m_H-1);

//     for (int y=tl.y; y<tl.y+sp.y; y++)
//     {
//         if (y >= m_H-1)
//         {
//             break;
//         }

//         for (int x=tl.x; x<tl.x+sp.y; x++)
//         {
//             if (x >= m_W-1)
//             {
//                 break;
//             }

//             // if (z < m_depthbuffer[y][x])
//             {
//                 m_backbuffer[y][x] = color;
//                 // m_depthbuffer[y][x] = z;
//             }
//         }
//     }
// }


// void
// idk::Video::blit( ivec2 dst, ivec2 src, ivec2 sp, const buf_wrapper<uint32_t> &buf, float z )
// {
//     int xmin = std::max(dst.x, 0);
//     int xmax = std::min(dst.x+sp.x, m_W-1);
//     int ymin = std::max(dst.y, 0);
//     int ymax = std::min(dst.y+sp.y, m_H-1);

//     for (int y=ymin, sy=src.y; y<ymax; y++, sy++)
//     {
//         for (int x=xmin, sx=src.x; x<xmax; x++, sx++)
//         {
//             // if (z < m_depthbuffer[y][x])
//             {
//                 m_backbuffer[y][x] = buf[sy][sx];
//                 // m_depthbuffer[y][x] = z;
//             }
//         }
//     }
// }



// void
// idk::Video::loadFonts( limine_module_response *res )
// {
//     syslog log("idk::Video::loadFonts");
//     log("module_count=%d\n", res->module_count);

//     for (size_t i=0; i<res->module_count; i++)
//     {
//         auto *file = res->modules[i];

//         if (strncmp(file->string, "font", 4) == 0)
//         {
//             log("font: \"%s\"\n", file->string);
//             auto *header = (BMP_header*)file->address;
//             m_fonts.push_back(FontBuffer(file->string, header));
//         }
//     }

// }



// // idk::video::FONTS&
// // idk::video::getFonts()
// // {
// //     return __video_fonts;
// // }

