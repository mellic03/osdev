// #include "ksystem.hpp"
#include "kshell.hpp"
#include "kfs/kfs.hpp"
#include "tty.hpp"
#include "log/log.hpp"
#include "driver/serial.hpp"
#include "driver/keyboard.hpp"
#include <kmalloc.h>
#include <kstring.h>
#include <kscancode.h>
#include <kfile.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <kfs.hpp>

using namespace idk;
using namespace KFS;


enum Command: uint32_t
{
    CMD_INVALID = 0,
    CMD_LS,
    CMD_LSMEM,
    CMD_LSGFX,
    CMD_EXIT
};


using fn_type = char *(*)( char *dst, const char *src );

char *cmd_unknown( char *, const char* );
char *cmd_cwd( char*, const char* );
char *cmd_ls( char*, const char* );
char *cmd_cd( char*, const char* );
char *cmd_tid( char*, const char* );
char *cmd_clear( char*, const char* );
char *cmd_exit( char*, const char* );



const char *cmd_names[] = {
    "cwd",
    "ls",
    "cd",
    "tid",
    "clear",
    // "lsmem",
    // "lsgfx",
    "exit"
};


const fn_type cmd_methods[] = {
    cmd_cwd,
    cmd_ls,
    cmd_cd,
    cmd_tid,
    cmd_clear,
    // cmd_unknown,
    // cmd_unknown,
    cmd_exit
};


static char  ksbuf[64];
static kTTY *kshell_tty;
static char  ksindent[] = "    ";
// #define INDENT for (int xu=0; xu<kshell_indent; xu++) dst += sprintf(dst, " ")

void kshell_interpret( kTTY *tty )
{
    kshell_tty = tty;

    if (tty->cwd == nullptr)
    {
        tty->cwd = KFS::insertDirectory("home/tty0/");
        KFS::insertDirectory("home/tty0/dir0/");
        KFS::insertDirectory("home/tty0/dir1/");
        KFS::insertDirectory("home/tty0/dir2/");
    }

    char *&dst = tty->htop;
    const char *src = tty->prompt;

    if (strlen(src) == 0)
    {
        return;
    }

    auto *A = skip_brk(src, " \n");
    auto *B = seek_brk(A, " \n");
    strncpy(ksbuf, A, B-A);

    // kshell_indent += 4;
    int count = sizeof(cmd_names) / sizeof(char*);

    for (int i=0; i<count; i++)
    {
        if (strcmp(ksbuf, cmd_names[i]) == 0)
        {
            dst = cmd_methods[i](dst, B);
            return;
        }
    }

    dst += sprintf(dst, "%sunrecognised command \"%s\"\n", ksindent, ksbuf);
}



char *cmd_unknown( char *dst, const char* )
{
    return strcpy(dst, "unrecognised command\n");
}


char *cmd_cwd( char *dst, const char *src )
{
    auto *&cwd = kshell_tty->cwd;
    dst += sprintf(dst, "%s%s\n", ksindent, cwd->get_path());
    return dst;
}


char *cmd_ls( char *dst, const char *src )
{
    auto *&cwd = kshell_tty->cwd;

    auto *dirname = ksbuf;
    auto *A = skip_brk(src, " \n");
    auto *B = seek_brk(A, " \n");
    strncpy(dirname, A, B-A);

    auto *dir = KFS::findDirectory(dirname);

    if (dir)
    {
        for (auto *E: dir->children)
        {
            dst += sprintf(dst, "%s%s\n", ksindent, E->name);
        }
    }

    // dst += sprintf(dst, "%s%s\n", ksindent, A);


    return dst;
}


char *cmd_cd( char *dst, const char *src )
{
    auto *&cwd = kshell_tty->cwd;

    char *A = (char*)skip_brk(src, " ");
    char *B = (char*)seek_brk(A, " \n");
    *B = '\0';

    dst += sprintf(dst, "--cd \"%s\"\n", A);
    // for (auto *E: cwd->children)
    // {
    //     INDENT;
    //     dst += sprintf(dst, "%s/\n", E->name);
    // }

    return dst;
}


char *cmd_tid( char *dst, const char *src )
{
    size_t tid = kthread::this_tid();
    int n = sprintf(dst, "%stid: %lu\n", ksindent, tid);
    return dst + n;
}


char *cmd_clear( char *dst, const char *src )
{
    kshell_tty->clear();
    return kshell_tty->history;
}


char *cmd_exit( char *dst, const char *src )
{
    kthread::exit();
    return dst;
}



