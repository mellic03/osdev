#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;


char *kshell_ls( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    vfsDirEntry *dir;

    if (argc == 1)
        dir = cwd;
    else
        dir = KFS::findDirectory(argv[1]);

    if (dir)
    {
        for (auto *E: dir->children)
        {
            dst = kssprintf(dst, "%s", E->name);
        }
    }

    else
    {
        char *dirname = argv[1];
        dst = kssprintf(dst, "could not find directory \"%s\"", dirname);
    }

    return dst;
}

