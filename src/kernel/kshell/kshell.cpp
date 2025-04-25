#include "kshell.hpp"
#include <kernel/vfs.hpp>
#include "../sde/sde.hpp"
#include "../sde/frame_text.hpp"
#include "../sde/button.hpp"
#include "../driver/keyboard.hpp"
#include <kpanic.h>
#include <kstring.h>
#include <kmalloc.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace idk;

using namespace KShell;
using argv_type = char[16][32];


kTTY *kshell_tty;
static argv_type kshell_argv;
// int KShell::kshell_indent = 2;
// void KShell::pushIndent() { kshell_indent += 2; }
// void KShell::popIndent()  { kshell_indent -= 2; }



extern size_t kshell_argparser( const char*, char[16][32] );
extern char *kshell_cwd    ( char*, int, char[16][32] );
extern char *kshell_cd     ( char*, int, char[16][32] );
// extern char *kshell_exec   ( char*, int, char[16][32] );
extern char *kshell_font   ( char*, int, char[16][32] );
extern char *kshell_info   ( char*, int, char[16][32] );
extern char *kshell_ls     ( char*, int, char[16][32] );
extern char *kshell_mkdir  ( char*, int, char[16][32] );
extern char *kshell_set    ( char*, int, char[16][32] );
extern char *kshell_panic  ( char*, int, char[16][32] );
// extern char *kshell_send   ( char*, int, char[16][32] );
// extern char *kshell_sendq  ( char*, int, char[16][32] );
// extern char *kshell_sendmsg( char*, int, char[16][32] );
extern char *kshell_tid    ( char*, int, char[16][32] );
extern char *kshell_clear  ( char*, int, char[16][32] );
extern char *kshell_exit   ( char*, int, char[16][32] );

using fn_type = char *(*)( char*, int, char[16][32] );
struct command_pair { const char *name; fn_type fn; };

static std::vector<command_pair> cmd_table;
#define KSHELL_REGISTER_CMD(name) cmd_table.push_back({#name, kshell_##name});

static void registerCommands()
{
    KSHELL_REGISTER_CMD(cwd)
    KSHELL_REGISTER_CMD(cd)
    // KSHELL_REGISTER_CMD(exec)
    KSHELL_REGISTER_CMD(font)
    KSHELL_REGISTER_CMD(info)
    KSHELL_REGISTER_CMD(ls)
    KSHELL_REGISTER_CMD(mkdir)
    KSHELL_REGISTER_CMD(set)
    KSHELL_REGISTER_CMD(panic)
    // KSHELL_REGISTER_CMD(send)
    // KSHELL_REGISTER_CMD(sendq)
    // KSHELL_REGISTER_CMD(sendmsg)
    KSHELL_REGISTER_CMD(tid)
    KSHELL_REGISTER_CMD(clear)
    KSHELL_REGISTER_CMD(exit)
}


// __attribute__((constructor))
// static void kshell_init()
// {
//     // for (int i=0; i<MAX_ARG_COUNT; i++)
//     // {
//     //     kshell_argv[i] = (char*)kmalloc(MAX_ARG_LENGTH*sizeof(char));
//     // }
// }




void kshell_parse( kTTY *tty )
{
    kshell_tty = tty;
    size_t argc = kshell_argparser(kshell_tty->prompt, kshell_argv);

    if (argc == 0)
    {
        return;
    }

    char *&dst = kshell_tty->rtop;

    for (auto &[name, fn]: cmd_table)
    {
        if (strcmp(name, kshell_argv[0]) == 0)
        {
            dst = fn(dst, argc, kshell_argv);
            return;
        }
    }

    dst = kssprintln(dst, "unrecognised command \"%s\"", kshell_argv[0]);
}




char *kshell_unknown( char *dst, int, char[16][32] )
{
    return strcpy(dst, "unrecognised command\n");
}


char *kshell_cwd( char *dst, int, char[16][32] )
{
    auto &cwd = kshell_tty->getCWD();
    dst = kssprintln(dst, "%s", cwd->get_path());
    return dst;
}



char *kshell_tid( char *dst, int, char[16][32] )
{
    size_t tid = kthread::this_tid();
    dst = kssprintln(dst, "tid: %lu", tid);
    return dst;
}


char *kshell_clear( char *dst, int, char[16][32] )
{
    kshell_tty->clear();
    return dst;
}


char *kshell_exit( char *dst, int, char[16][32] )
{
    kshell_tty->running = false;
    return dst;
}









using namespace kdriver::ps2_kb;
#include <new>
#include "../cpu/smp.hpp"
extern kfstream *cpu_streams[4];

void kshell_main( void* )
{
    registerCommands();
    kTTY tty(25);
    tty.font = new sde::Font("/font/cutive-w12hf18.bmp");

    auto *stream = &(vfsFindFile("dev/kb0/event")->stream);
    auto *win = sde::createWindow(ivec2(10, 10), ivec2(600, 600));
    win->m_style = {
        .fill = true,
        .border = 4,
        .fill_color = vec4(0.2, 0.2, 0.2, 1.0),
        .border_color = vec4(1.0, 0.8, 0.8, 1.0)
    };

    auto *win2 = sde::createWindow(ivec2(10, 10), ivec2(300, 300));
    win2->m_style = {
        .fill = true,
        .border = 8,
        .fill_color = vec4(0.0, 0.8, 0.5, 1.0),
        .border_color = vec4(0.2, 0.2, 0.2, 1.0)
    };


    auto *ttyframe = new sde::TerminalFrame(20, 20, 400, 400, &tty);
    ttyframe->m_style = {
        .fill = true,
        .border = 4,
        .fill_color = vec4(0.9, 0.2, 0.2, 1.0),
        .border_color = vec4(0.0, 0.5, 0.9, 1.0)
    };
    win->m_children.push_back(ttyframe);

    auto *button = new sde::Button(500, 50, 100, 75);
    win->m_children.push_back(button);


    KeyEvent event;

    while (tty.running)
    {
        if (stream->read(&event, sizeof(KeyEvent)))
        {
            uint8_t  mask  = event.mask;
            uint8_t  ch    = event.key;
        
            if ((mask & KeyEvent_L) && (mask & KeyEvent_UP))
            {
                win->m_corner.x -= 50;
                win->make_dirty();
                // tty.moveCursor(-1);
            }

            else if ((mask & KeyEvent_R) && (mask & KeyEvent_UP))
            {
                win->m_corner.x += 50;
                win->make_dirty();
                // tty.moveCursor(+1);
            }

            else if (ch == '\n')
            {
                tty.submit(kshell_parse);
                win->make_dirty();

            }

            else if (ch != '\0')
            {
                tty.pputc(ch);
                win->make_dirty();
            }
        }
    
        kthread::yield();
    }

    sde::destroyWindow(win);
    sde::destroyWindow(win2);
}


