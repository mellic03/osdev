#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;


char *kshell_info( char *dst, int argc, char **argv )
{
    syslog log("kshell_info");

    auto &cwd = kshell_tty->getCWD();

    if (argc == 1)
    {
        dst = kssprintf(dst, "info: argument required");
        return dst;
    }

    char *fname = argv[1];
    auto *file  = kfilesystem::vfsFindFile(cwd, fname);

    if (file)
    {
        dst = kssprintf(dst, log, "name:  %s",    file->name.c_str());
        dst = kssprintf(dst, log, "type:  %s",    vfsFileTypeStr(file->type));
        // dst = kssprintf(dst, "end:   0x%lx", file->eof);
        // dst = kssprintf(dst, "read   0x%lx", *(file->read));
        // dst = kssprintf(dst, "write: 0x%lx", *(file->write));
        dst = kssprintf(dst, log, "size:  0x%lx", file->size);
    }

    else
    {
        dst = kssprintf(dst, log, "info: could not find file \"%s\"", fname);
    }

    return dst;
}

