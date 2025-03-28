#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;


char *kshell_info( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    vfsDirEntry *dir;

    if (argc == 1)
    {
        dst = kssprintf(dst, "info: argument required");
        return dst;
    }

    char *fname = argv[1];
    auto *file  = KFS::findFile(cwd, fname);

    if (file)
    {
        dst = kssprintf(dst, "%s", file->name);
        dst = kssprintf(dst, "base:  0x%lx", file->base);
        dst = kssprintf(dst, "end:   0x%lx", file->eof);
        dst = kssprintf(dst, "read   0x%lx", *(file->read));
        dst = kssprintf(dst, "write: 0x%lx", *(file->write));
        dst = kssprintf(dst, "size:  0x%lx", file->size);
    }

    else
    {
        dst = kssprintf(dst, "inf: could not find file \"%s\"", fname);
    }

    return dst;
}

