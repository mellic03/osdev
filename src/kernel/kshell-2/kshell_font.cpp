#include "kshell.hpp"
#include "../ksystem.hpp"
#include <kernel/vfs.hpp>
#include <kstring.h>
#include <stdlib.h>

using namespace KShell;


char *kshell_font( char *dst, int argc, char argv[16][32]  )
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


    // char *fname = argv[1];
    // auto *file  = vfsFindFile(cwd, fname);

    // if (file)
    // {
    // //     if (file->other == nullptr)
    // //     {
    // //         kvideo::Texture_rgba8u fontbmp((BMP_header*)(file->addr));
    // //         file->other = new sde::Font(fontbmp);
    // //     }
    // //     kshell_tty->font = (sde::Font*)(file->other);
    // }

    // else
    // {
    //     dst = kssprintln(dst, "font: could not find file \"%s\"", fname);
    // }

    return dst;
}

