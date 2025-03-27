// #include "ksystem.hpp"
#include "kshell.hpp"
#include "kfs/kfs.hpp"
#include "tty.hpp"
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
char *cmd_tid( char*, const char* );
char *cmd_exit( char*, const char* );



const char *cmd_names[] = {
    "cwd",
    "ls",
    "tid",
    // "lsmem",
    // "lsgfx",
    "exit"
};


const fn_type cmd_methods[] = {
    cmd_cwd,
    cmd_ls,
    cmd_tid,
    // cmd_unknown,
    // cmd_unknown,
    cmd_exit
};




char *kshell_interpret( char *dst, const char *src )
{
    if (strlen(src) == 0)
    {
        return dst;
    }

    char  buf[64];
    memset(buf, 0, sizeof(buf));

    char *top = dst;
    auto *A = skip_brk(src, " \n");
    auto *B = seek_brk(A, " \n");
    strncpy(buf, A, B-A);

    int count = sizeof(cmd_names) / sizeof(char*);

    for (int i=0; i<count; i++)
    {
        if (strcmp(buf, cmd_names[i]) == 0)
        {
            return cmd_methods[i](top, B);
        }
    }

    return cmd_unknown(top, B);
}



char *cmd_unknown( char *dst, const char* )
{
    return strcpy(dst, "unrecognised command");
}


char *cmd_cwd( char *dst, const char *src )
{
    return strcpy(dst, "/");
}

char *cmd_ls( char *dst, const char *src )
{
    return strcpy(dst, "--ls");
}


char *cmd_tid( char *dst, const char *src )
{
    size_t tid = kthread::this_tid();
    int n = sprintf(dst, "tid: %lu", tid);
    return dst + n;
}


char *cmd_exit( char *dst, const char *src )
{
    kthread::exit();
    return dst;
}



