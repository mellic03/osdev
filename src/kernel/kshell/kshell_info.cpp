#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;


char *kshell_info( char *dst, int argc, char **argv )
{
    syslog log("kshell_info");

    auto &cwd = kshell_tty->getCWD();

    if (argc == 1)
    {
        dst = kssprintln(dst, "info: argument required");
        return dst;
    }

    char *fname = argv[1];
    auto *file  = vfsFindFile(cwd, fname);

    if (file)
    {
        dst = kssprintln(dst, log, "name:  %s",    file->name);
        dst = kssprintln(dst, log, "size:  0x%lx", file->size);
        // dst = kssprint  (dst, log, "flags: %s",    vfsFileTypeStr(file->flags));
        dst = kssprintln(dst, log, "flags:");
        kshell_indent += 4;

        for (int i=1; i<=vfsFileFlag_MaxBit; i++)
        {
            if (file->flags & (1<<i))
            {
                dst = kssprint(dst, log, "--%s\n", vfsFileFlagStr((1<<i)));
            }
        }

        kshell_indent -= 4;

        // dst = kssprintln(dst, "end:   0x%lx", file->eof);
        // dst = kssprintln(dst, "read   0x%lx", *(file->read));
        // dst = kssprintln(dst, "write: 0x%lx", *(file->write));
    }

    else
    {
        dst = kssprintln(dst, log, "info: could not find file \"%s\"", fname);
    }

    return dst;
}

