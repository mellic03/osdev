#include "kvideo/kvideo.hpp"
#include "kwin/kwin.hpp"
#include "kwin/frame_text.hpp"
#include "driver/mouse.hpp"
#include "ksystem.hpp"

#include <stdio.h>


// kwin::Context *context;
// idk::KSystem *system;

// void ttyframe_main( void *arg )
// {
//     kTTY *tty = (kTTY*)arg;

//     auto *frame = context->createFrame<kwin::TerminalFrame>(
//         ivec2(10, 10), ivec2(500, 450),
//         &(system->m_fonts[6]), tty, kwin::Style(vec4(1.0f))
//     );

//     bool grabbing = false;
//     vec2 start = vec2(0.0f);

//     while (true)
//     {
//         if (mouseleft && !grabbing)
//         {
//             start = mousexy;
//             grabbing = true;
//         }

//         if (!mouseleft)
//         {
//             grabbing = false;
//         }

//         if (grabbing)
//         {
//             float dx = (mousexy.x - start.x);
//             float dy = (mousexy.y - start.y);

//             start.x = mousexy.x;
//             start.y = mousexy.y;

//             frame->m_local.x += dx;
//             frame->m_local.y += dy;
//         }

//         kthread::yield();
//     }
// }



#include "kshell/kshell.hpp"

// void video_main( void *ptr )
// {
//     u64vec2 args = *(u64vec2*)ptr;

//     kvideo::init(args[0]);
//     system  = (idk::KSystem*)args[1];

//     auto ctx = kwin::Context(1280, 720);
//     context = &ctx;

//     kthread t0(kshell_main, system->tty0);
//     // kthread t1(ttyframe_main, system->tty0);


//     while (true)
//     {
//         ctx.rectOutline(ivec2(0, 0), ctx.m_sp, vec4(1.0, 0.0, 0.0, 1.0));
    
//         for (auto *F: ctx.frames)
//         {
//             F->updateTransforms();
//             F->draw(ctx);
//         }

//         if (mouseleft)
//         {
//             ctx.rect(mousexy, vec2(24, 24), vec4(0.5f));
//         }
    
//         else
//         {
//             ctx.rect(mousexy, vec2(24, 24), vec4(1.0f));
//         }

//         kvideo::blit(ctx.m_tl, vec2(0, 0), ctx.m_sp, ctx.m_fb);
//         kmemset<vec4>(ctx.m_fb.buf, vec4(0.0f), ctx.m_sp.x * ctx.m_sp.y);
//         kvideo::swapBuffers();

//         // // printf("[video_main]\n");
//         // if (uptime_ms > 30)
//         // {
//         //     kvideo::swapBuffers();
//         //     uptime_ms = 0;
//         // }
//         // else
//         // {
//         //     kvideo::blit(ctx.m_tl, vec2(0, 0), ctx.m_sp, ctx.m_fb);
//         //     kmemset<vec4>(ctx.m_fb.buf, vec4(0.0f), ctx.m_sp.x * ctx.m_sp.y);
//         // }

//         kthread::yield();
//     }

// }


