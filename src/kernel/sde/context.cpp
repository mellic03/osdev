// #ifndef __is_kernel
//     #define __is_kernel
// #endif

// #include "context.hpp"
// #include "frame.hpp"
// #include "sde.hpp"
// #include <kthread.hpp>
// #include <stdio.h>
// #include <algorithm>
// #include <kernel/vfs.hpp>
// #include <kernel/log.hpp>
// // #include <kmalloc.h>



// sde::WindowContext::WindowContext( ivec2 tl, ivec2 sp )
// :   Frame(tl.x, tl.y, sp.x, sp.y),
//     ID(ctx_id++),
//     W(sp.x), H(sp.y)
// {
//     memset(title, '\0', sizeof(title));

//     // ivec2 corner  = ivec2(0, 0) + m_style.margin[0];
//     // int   menuw   = m_span.x - (m_style.margin[0] + m_style.margin[1]);
//     // int   menuh   = m_style.margin[2];

//     // auto *menubar = new sde::Frame(corner, ivec2(menuw, menuh));
//     // menubar->m_style = {
//     //     .fill         = true,
//     //     .border       = 4,
//     //     .fill_color   = vec4(0.8, 0.8, 0.8, 1.0),
//     //     .border_color = vec4(0.75)
//     // };

//     // m_children.push_back(menubar);
// }


// sde::WindowContext::~WindowContext()
// {
//     // kfree(rgba->data);
//     // kfree(rgba.buf);
//     // kfree(depth.buf);
// }

// // void sde::WindowContext::update()
// // {
// //     m_local = ivec2(0, 0);

// //     if (m_parent)
// //     {
// //         m_global += m_parent->m_global;
// //     }

// //     for (Frame *F: m_children)
// //     {
// //         F->z = this->z - 0.01f;
// //         F->update();
// //     }
 
// //     if (m_global != m_prev_global)
// //     {
// //         make_dirty();
// //     }

// //     m_prev_global = m_global;
// // }

// void sde::WindowContext::draw()
// {
//     // if (m_style.fill || m_style.border)
//     // {

//     // int title_h = 32;
//     // float depth = this->z;

//     // gxDrawRect(
//     //     sde::getCurrent()->rgba,
//     //     {0, 0, m_span.x, title_h, depth},
//     //     false, 2,
//     //     vec4(0.2, 0.8, 0.8, 1.0),
//     //     m_style.border_color
//     // );
//     // depth -= 0.01f;

//     // sde::sysfont_putchar('E', 10, 10, depth);

//     m_wcorner = m_corner;
//     if (m_parent) { m_wcorner += m_parent->m_corner; }

//     gxDrawRect(
//         {m_wcorner.x, m_wcorner.y, m_span.x, m_span.y, m_depth},
//         m_style.fill, m_style.border,
//         m_style.fill_color,
//         m_style.border_color
//     );

//     for (Frame *F: m_children)
//     {
//         F->m_depth =  m_depth - 0.01f;
//         F->draw();
//     }

// }


// // void sde::WindowContext::clear()
// // {
// //     kmemset<vec4>(rgba.buf, 0, W*H*sizeof(vec4));
// // }
