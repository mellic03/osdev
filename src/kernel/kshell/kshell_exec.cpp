#include "kshell.hpp"
#include "../ksystem.hpp"
#include <kstring.h>
using namespace KShell;


extern idk::KSystem *sys;


char *kshell_exec( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    vfsDirEntry *dir;

    if (argc == 1)
    {
        dst = kssprintf(dst, "exec: argument required");
        return dst;
    }

    char *fname = argv[1];
    auto *file  = KFS::findFile(cwd, fname);

    if (file)
    {
        int res = sys->execute(file->base, file->size, 0, nullptr);
        dst = kssprintf(dst, "result: %d", res);
    }

    else
    {
        dst = kssprintf(dst, "exec: could not find file \"%s\"", fname);
    }

    return dst;
}

