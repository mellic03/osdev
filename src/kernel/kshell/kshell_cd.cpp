#include "kshell.hpp"
#include <kstring.h>
#include <string.h>
using namespace KShell;



vfsDirEntry *kshell_getdir( vfsDirEntry *cwd, char *name )
{
    vfsDirEntry *dir = nullptr;

    if (strcmp(name, "..") == 0)
        dir = (cwd->parent) ? cwd->parent : cwd;
    else
        dir = KFS::findDirectory(cwd, name);

    return dir;
}


char *kshell_cd( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    if (argc == 1)
    {
        dst = kssprintf(dst, "cd: no arguments");
        return dst;
    }

    vfsDirEntry *dir = kshell_getdir(cwd, argv[1]);

    if (dir)
        cwd = dir;
    else
        dst = kssprintf(dst, "cd: could not find directory \"%s\"", argv[1]);

    return dst;
}

