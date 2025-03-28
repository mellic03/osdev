#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;


char *kshell_info( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    if (argc == 1)
    {
        dst = kssprintf(dst, "info: argument required");
        return dst;
    }

    char *fname = argv[1];
    auto *file  = KFS::findFile(cwd, fname);

    if (file)
    {
        dst = kssprintf(dst, "name:  %s",    file->name);
        dst = kssprintf(dst, "type:  %s",    vfsFileTypeStr(file->type));
        // dst = kssprintf(dst, "end:   0x%lx", file->eof);
        // dst = kssprintf(dst, "read   0x%lx", *(file->read));
        // dst = kssprintf(dst, "write: 0x%lx", *(file->write));
        dst = kssprintf(dst, "size:  0x%lx", file->size);
    }

    else
    {
        dst = kssprintf(dst, "info: could not find file \"%s\"", fname);
    }

    return dst;
}

