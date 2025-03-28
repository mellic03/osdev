#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;


extern vfsDirEntry *kshell_getdir( vfsDirEntry*, char* );

char *kshell_ls( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    vfsDirEntry *dir = nullptr;

    if (argc == 1)
        dir = cwd;
    else
        dir = kshell_getdir(cwd, argv[1]);

    if (dir)
    {
        for (auto *E: dir->children)
        {
            if (E->is_dir())
                dst = kssprintf(dst, "%s", E->name);
        }

        for (auto *E: dir->children)
        {
            if (E->is_file())
                dst = kssprintf(dst, "%s", E->name);
        }
    }

    else
    {
        dst = kssprintf(dst, "ls: could not find directory \"%s\"", argv[1]);
    }

    return dst;
}

