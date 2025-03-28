#include "kshell.hpp"
#include "kfs/kfs.hpp"
#include "../kwin/kwin.hpp"
#include "../kwin/frame_text.hpp"
#include "../driver/keyboard.hpp"
#include <kstring.h>
#include <kmalloc.h>
#include <stdio.h>
#include <string.h>
#include <idk_vector.hpp>
#include <algorithm>

using namespace idk;
using namespace KFS;

using namespace KShell;
using argv_type = char[16][32];

extern size_t kshell_argparser( const char*, char** );
extern char *kshell_cwd    ( char*, int, char** );
extern char *kshell_ls     ( char*, int, char** );
extern char *kshell_cd     ( char*, int, char** );
extern char *kshell_exec   ( char*, int, char** );
extern char *kshell_font   ( char*, int, char** );
extern char *kshell_info   ( char*, int, char** );
extern char *kshell_tid    ( char*, int, char** );
extern char *kshell_clear  ( char*, int, char** );
extern char *kshell_exit   ( char*, int, char** );


using fn_type = char *(*)( char*, int, char** );

const char *cmd_names[] = {
    "cwd",
    "ls",
    "cd",
    "exec",
    "font",
    "info",
    "tid",
    "clear",
    "exit"
};

const fn_type cmd_methods[] = {
    kshell_cwd,
    kshell_ls,
    kshell_cd,
    kshell_exec,
    kshell_font,
    kshell_info,
    kshell_tid,
    kshell_clear,
    kshell_exit
};


kTTY *KShell::kshell_tty;
char  KShell::kshell_buf[128];
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
    int   count = sizeof(cmd_names) / sizeof(char*);

    for (int i=0; i<count; i++)
    {
        if (strcmp(name, cmd_names[i]) == 0)
        {
            dst = cmd_methods[i](dst, argc, kshell_argv);
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




void kshell_main( void *arg )
{
    using namespace kdriver::ps2_kb;
    kshell_argv = (char**)kmalloc(MAX_ARG_COUNT);
    for (int i=0; i<MAX_ARG_COUNT; i++)
    {
        kshell_argv[i] = (char*)kmalloc(MAX_ARG_LENGTH);
    }

    kTTY *tty    = (kTTY*)arg;
    auto *stream = (kfstream*)(KFS::findFile("dev/kb0/event")->addr);

    auto *ctx = kwin::createContext(550, 500);
    auto *root = ctx->createFrame<kwin::Frame>(
        ivec2(10, 10), ivec2(500, 450), vec4(1.0f), kwin::Style(vec4(1.0f, 1.0, 0.8f, 1.0f))
    );
    root->giveChild<kwin::TerminalFrame>(
        ivec2(25, 25), ivec2(480, 420), tty, vec4(0.0f)
    );

    KeyEvent event;

    while (tty->running)
    {
        if (stream->read(&event, sizeof(KeyEvent)))
        {
            uint8_t  mask  = event.mask;
            uint8_t  ch    = event.key;
        
            if ((mask & KeyEvent_L) && (mask & KeyEvent_UP))
            {
                tty->moveCursor(-1);
            }

            else if ((mask & KeyEvent_R) && (mask & KeyEvent_UP))
            {
                tty->moveCursor(+1);
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
    }

    kwin::destroyContext(ctx);

}


