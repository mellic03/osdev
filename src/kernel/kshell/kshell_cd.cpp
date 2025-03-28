#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;


char *kshell_cd( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    // auto *dirname = kshell_buf;
    // auto *A = skip_brk(src, " \n");
    // auto *B = seek_brk(A, " \n");
    // strncpy(dirname, A, B-A);

    // vfsDirEntry *dir;

    if (argc == 1)
    {
        dst = kssprintf(dst, "cd: no arguments");
        return dst;
    }

    char *dirname = argv[1];
    auto *dir = KFS::findDirectory(cwd, dirname);

    if (dir)
    {
        cwd = dir;
    }

    else
    {
        dst = kssprintf(dst, "cd: could not find directory \"%s\"", dirname);
    }

    return dst;
}

