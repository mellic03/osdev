#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;


char *kshell_info( char *dst, int argc, char argv[16][32]  )
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

    if (!file)
    {
        return kssprintln(dst, "info: could not find file \"%s\"", fname);
    }

    dst = kssprintln(dst, "name:  %s",    file->name);
    dst = kssprintln(dst, "size:  0x%lx", file->size);
    dst = kssprintln(dst, "flags:");
    // kshell_indent += 4;

    for (int i=1; i<=vfsFileFlag_MaxBit; i++)
    {
        if (file->flags & (1<<i))
        {
            dst = kssprint(dst, "--%s\n", vfsFileFlagStr((1<<i)));
        }
    }

    if (file->flags & vfsFileFlag_Text)
    {
        dst = kssprint(dst, "content:\n%s\n", (char*)(file->addr));
    }



    size_t len = strlen(file->name);
    // dst = kssprint(dst, "bruh: \'%c\'\n", file->name[len-4]);

    if (strcmp(file->name + len-4, ".bmp") == 0)
    {
        auto *bmp  = (BMP_header*)(file->addr);
        auto &info = bmp->infoheader;
    
        dst = kssprint(dst, "width:       %u\n", info.width);
        dst = kssprint(dst, "height:      %u\n", info.height);
        dst = kssprint(dst, "bpp:         %u\n", info.bpp);
        dst = kssprint(dst, "planes:      %u\n", info.planes);
        dst = kssprint(dst, "imsize:      %u\n", info.imagesize);
        dst = kssprint(dst, "compression: %s\n", BI_string(info.compression));
        dst = kssprint(dst, "c_used:      %u\n", info.colors_used);
        dst = kssprint(dst, "c_imp:       %u\n", info.colors_important);
    }

    // kshell_indent -= 4;

    // dst = kssprintln(dst, "end:   0x%lx", file->eof);
    // dst = kssprintln(dst, "read   0x%lx", *(file->read));
    // dst = kssprintln(dst, "write: 0x%lx", *(file->write));

    return dst;
}

