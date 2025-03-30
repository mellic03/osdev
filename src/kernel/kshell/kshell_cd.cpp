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
        dir = kfilesystem::vfsFindDirectory(cwd, name);

    return dir;
}


char *kshell_cd( char *dst, int argc, char **argv )
{
    syslog log("kshell_cd");

    auto &cwd = kshell_tty->getCWD();

    if (argc == 1)
    {
        dst = kssprintf(dst, log, "cd: no arguments");
        return dst;
    }

    vfsDirEntry *dir = kshell_getdir(cwd, argv[1]);

    if (dir)
        cwd = dir;
    else
        dst = kssprintf(dst, log, "cd: could not find directory \"%s\"", argv[1]);

    return dst;
}

