#include "kshell.hpp"
#include <kernel/vfs.hpp>
#include "../sde/sde.hpp"
#include "../sde/frame_text.hpp"
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


int KShell::kshell_indent = 2;
void KShell::pushIndent() { kshell_indent += 2; }
void KShell::popIndent()  { kshell_indent -= 2; }



extern size_t kshell_argparser( const char*, char** );
extern char *kshell_cwd    ( char*, int, char** );
extern char *kshell_cd     ( char*, int, char** );
// extern char *kshell_exec   ( char*, int, char** );
extern char *kshell_font   ( char*, int, char** );
extern char *kshell_info   ( char*, int, char** );
extern char *kshell_ls     ( char*, int, char** );
extern char *kshell_mkdir  ( char*, int, char** );
extern char *kshell_set    ( char*, int, char** );
extern char *kshell_panic  ( char*, int, char** );
extern char *kshell_send   ( char*, int, char** );
extern char *kshell_sendmsg( char*, int, char** );
extern char *kshell_tid    ( char*, int, char** );
extern char *kshell_clear  ( char*, int, char** );
extern char *kshell_exit   ( char*, int, char** );

using fn_type = char *(*)( char*, int, char** );
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
    KSHELL_REGISTER_CMD(send)
    KSHELL_REGISTER_CMD(sendmsg)
    KSHELL_REGISTER_CMD(tid)
    KSHELL_REGISTER_CMD(clear)
    KSHELL_REGISTER_CMD(exit)
}

kTTY *kshell_tty;
static char *kshell_argv[MAX_ARG_COUNT];

// __attribute__((constructor))
// static void kshell_init()
// {
//     for (int i=0; i<MAX_ARG_COUNT; i++)
//     {
//         kshell_argv[i] = (char*)kmalloc(MAX_ARG_LENGTH*sizeof(char));
//     }
// }




void kshell_parse()
{
    syslog log("kshell_parse");

    // if (*kshell_argv == nullptr)
    // {
    //     kthread::yield_guard lock();
    //     kshell_argv = (char**)kmalloc(MAX_ARG_COUNT*sizeof(char*));
        
    //     for (int i=0; i<MAX_ARG_COUNT; i++)
    //     {
    //         *(kshell_argv + i) = (char*)kmalloc(MAX_ARG_LENGTH*sizeof(char));
    //     }
    // }


    size_t argc = kshell_argparser(kshell_tty->prompt, kshell_argv);

    if (argc == 0)
    {
        return;
    }

    char *&dst = kshell_tty->htop;

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




char *kshell_unknown( char *dst, int, char** )
{
    return strcpy(dst, "unrecognised command\n");
}


char *kshell_cwd( char *dst, int, char** )
{
    auto &cwd = kshell_tty->getCWD();
    dst = kssprintln(dst, "%s", cwd->get_path());
    return dst;
}



char *kshell_tid( char *dst, int, char** )
{
    size_t tid = kthread::this_tid();
    dst = kssprintln(dst, "tid: %lu", tid);
    return dst;
}


char *kshell_clear( char*, int, char** )
{
    kshell_tty->clear();
    return kshell_tty->history;
}


char *kshell_exit( char *dst, int, char** )
{
    kshell_tty->running = false;
    return dst;
}









using namespace kdriver::ps2_kb;
#include <new>
#include <ipc.hpp>



void kshell_main( void* )
{
    syslog log("kshell_main");
    registerCommands();


    for (int i=255; i>=0; i--)
    {
        auto res = ipcport_open(0x05ED);
        if (res == PORT_ACCEPTED)
            break;
        else if (i==0)
            kthread::yield();
    }

    {
        kthread::yield_guard lock(kthread::lock_count);

        for (int i=0; i<MAX_ARG_COUNT; i++)
        {
            kshell_argv[i] = (char*)kmalloc(MAX_ARG_LENGTH*sizeof(char));
        }
    }


    kTTY tty(25*80);
    kshell_tty = &tty;

    auto *file = vfsFindFile("/font/cutive-w12hf18.bmp");
          file->flags |= vfsFileFlag_Stream;
          file->flags |= vfsFileFlag_Virtual;
    tty.font = new idk::FontBuffer((ck_BMP_header*)(file->addr));

    auto *stream = &(vfsFindFile("dev/kb0/event")->stream);

    auto *ctx = sde::createContext(ivec2(10, 10), ivec2(600, 600));
    ctx->giveChild(ivec2(10), new sde::TerminalFrame(&tty));

    ctx->m_style = {
        .fill = false,
        .border = false,
        .border_color = vec4(1.0, 0.8, 0.8, 1.0)
    };

    KeyEvent event;
    syslog::kprintf("[syslog] count: %d\n", kthread::lock_count.load());

    while (tty.running)
    {

        if (stream->read(&event, sizeof(KeyEvent)))
        {
            uint8_t  mask  = event.mask;
            uint8_t  ch    = event.key;
        
            if ((mask & KeyEvent_L) && (mask & KeyEvent_UP))
            {
                ctx->m_local.x -= 50;
                // tty->moveCursor(-1);
            }

            else if ((mask & KeyEvent_R) && (mask & KeyEvent_UP))
            {
                ctx->m_local.x += 50;
                // tty->moveCursor(+1);
            }

            else if (ch == '\n')
            {
                log("kshell_argv:  0x%lx", kshell_argv);
                log("*kshell_argv: 0x%lx", *kshell_argv);

                auto *&cwd = tty.getCWD();
                tty.hsprintf("[%s] ", cwd->name);

                if (tty.ptop == tty.prompt)
                {
                    tty.hputc('\n');
                    continue;
                }

                tty.hputs(tty.prompt);
                tty.hputc('\n');
                kshell_parse();
                tty.pclear();
            }

            else if (ch != '\0')
            {
                tty.pputc(ch);
            }
        }

        kthread::yield();
    }

    sde::destroyContext(ctx);

    for (int i=0; i<MAX_ARG_COUNT; i++)
        kfree(kshell_argv[i]);

}


