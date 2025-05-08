#include "kshell.hpp"
#include <kstring.h>
#include <string.h>
using namespace KShell;


char *kshell_mkdir( char *dst, int argc, char argv[16][32]  )
{
    syslog log("kshell_mkdir");
    // auto &cwd = kshell_tty->getCWD();

    if (argc == 1)
    {
        dst = kssprintln(dst, log, "mkdir: no arguments");
        return dst;
    }

    // auto *dir = vfs2::open(argv[1]);
    // if (dir) return kssprintln(dst, log, "mkdir: directory \"%s\" already exists", argv[1]);
    // dir = vfsInsertDirectory(argv[1]);

    return dst;
}

