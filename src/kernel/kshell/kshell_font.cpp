#include "kshell.hpp"
#include "../ksystem.hpp"
#include <kernel/vfs.hpp>
#include <kstring.h>
#include <stdlib.h>

using namespace KShell;


char *kshell_font( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    if (argc == 1)
    {
        dst = kssprintln(dst, "font: argument required");
        return dst;
    }

    else if (argc == 3)
    {
        if (strcmp(argv[1], "--scale") == 0)
        {
            float scale = 1.0f / float(atoi(argv[2]));
            kshell_tty->font->glyph_scale = vec2(scale, 1.0f);
            dst = kssprintln(dst, "font: scale set to \"%f\"", scale);
            return dst;
        }
    }


    char *fname = argv[1];
    auto *file  = kfilesystem::vfsFindFile(cwd, fname);

    if (file)
    {
        if (file->other == nullptr)
            file->other = new idk::FontBuffer((ck_BMP_header*)file->addr);
        kshell_tty->font = (idk::FontBuffer*)(file->other);
    }

    else
    {
        dst = kssprintln(dst, "font: could not find file \"%s\"", fname);
    }

    return dst;
}

