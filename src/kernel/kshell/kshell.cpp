#include "kshell.hpp"
#include <kernel/vfs.hpp>
#include "../sde/sde.hpp"
#include "../sde/frame_text.hpp"
#include "../driver/keyboard.hpp"
#include <kstring.h>
#include <kmalloc.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace idk;

using namespace KShell;
using argv_type = char[16][32];

extern size_t kshell_argparser( const char*, char** );
extern char *kshell_cwd    ( char*, int, char** );
extern char *kshell_cd     ( char*, int, char** );
extern char *kshell_exec   ( char*, int, char** );
extern char *kshell_font   ( char*, int, char** );
extern char *kshell_info   ( char*, int, char** );
extern char *kshell_ls     ( char*, int, char** );
extern char *kshell_set    ( char*, int, char** );
extern char *kshell_tid    ( char*, int, char** );
extern char *kshell_clear  ( char*, int, char** );
extern char *kshell_exit   ( char*, int, char** );


using fn_type = char *(*)( char*, int, char** );
struct command_pair { const char *name; fn_type fn; };

static std::vector<command_pair> cmd_table;
// static idk::static_vector<char[MAX_ARG_LENGTH], MAX_ARG_COUNT> kshell_argv;
#define KSHELL_REGISTER_CMD(name) cmd_table.push_back({#name, kshell_##name});

static void registerCommands()
{
    KSHELL_REGISTER_CMD(cwd)
    KSHELL_REGISTER_CMD(cd)
    KSHELL_REGISTER_CMD(exec)
    KSHELL_REGISTER_CMD(font)
    KSHELL_REGISTER_CMD(info)
    KSHELL_REGISTER_CMD(ls)
    KSHELL_REGISTER_CMD(set)
    KSHELL_REGISTER_CMD(tid)
    KSHELL_REGISTER_CMD(clear)
    KSHELL_REGISTER_CMD(exit)
}

kTTY *KShell::kshell_tty;
char  **kshell_argv;


static void
kshell_parse( kTTY *tty )
{
    kshell_tty = tty;

    size_t argc = kshell_argparser(tty->prompt, kshell_argv);

    if (argc == 0)
    {
        return;
    }

    char *&dst  = tty->htop;
    char *name  = kshell_argv[0];

    for (auto &[name, fn]: cmd_table)
    {
        if (strcmp(kshell_argv[0], name) == 0)
        {
            dst = fn(dst, argc, kshell_argv);
            return;
        }
    }

    dst = kssprintf(dst, "unrecognised command \"%s\"", kshell_argv[0]);
}




char *kshell_unknown( char *dst, int, char** )
{
    return strcpy(dst, "unrecognised command\n");
}


char *kshell_cwd( char *dst, int, char** )
{
    auto &cwd = kshell_tty->getCWD();
    dst = kssprintf(dst, "%s", cwd->get_path());
    return dst;
}



char *kshell_tid( char *dst, int, char** )
{
    size_t tid = kthread::this_tid();
    dst = kssprintf(dst, "tid: %lu", tid);
    return dst;
}


char *kshell_clear( char *dst, int, char** )
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

void kshell_main( void *arg )
{
    registerCommands();

    kshell_argv = new char*[MAX_ARG_COUNT];
    for (int i=0; i<MAX_ARG_COUNT; i++)
    {
        kshell_argv[i] = new char[MAX_ARG_LENGTH];
    }

    kTTY *tty    = (kTTY*)arg;
    auto *stream = &(kfilesystem::vfsFindFile("dev/kb0/event")->stream);

    auto *ctx = sde::createContext(ivec2(10, 10), ivec2(500, 500));
          ctx->giveChild(ivec2(10), new sde::TerminalFrame(tty));


    KeyEvent event;

    while (tty->running)
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
                auto *&cwd = tty->getCWD();
                tty->hsprintf("[%s] ", cwd->name);

                if (tty->ptop == tty->prompt)
                {
                    tty->hputc('\n');
                    continue;
                }

                tty->hputs(tty->prompt);
                tty->hputc('\n');
                kshell_parse(tty);
                tty->pclear();
            }

            else if (ch != '\0')
            {
                tty->pputc(ch);
            }
        }

        kthread::yield();
    }

    sde::destroyContext(ctx);

    for (int i=0; i<MAX_ARG_COUNT; i++)
        delete[] kshell_argv[i];
    delete[] kshell_argv;
}


