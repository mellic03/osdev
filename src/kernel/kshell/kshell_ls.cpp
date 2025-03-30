#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;


extern vfsDirEntry *kshell_getdir( vfsDirEntry*, char* );

char *kshell_ls( char *dst, int argc, char **argv )
{
    syslog log("kshell_ls");

    auto &cwd = kshell_tty->getCWD();
    vfsDirEntry *dir = nullptr;

    if (argc == 1)
        dir = cwd;
    else
        dir = kshell_getdir(cwd, argv[1]);

    if (dir)
    {
        for (auto *E: dir->children)
        {
            if (E->is_dir())
            {
                // dst = kssprintf(dst, "%s", E->name);
                dst = kssprintf(dst, log, "%s", E->name.c_str());
                // log("\"%s\"", E->name);
            }
        }

        for (auto *E: dir->children)
        {
            if (E->is_file())
            {
                // dst = kssprintf(dst, "%s", E->name);
                dst = kssprintf(dst, log, "%s", E->name.c_str());
                // log("\"%s\"", E->name);
            }
        }
    }

    else
    {
        // dst = kssprintf(dst, "ls: could not find directory \"%s\"", argv[1]);
        dst = kssprintf(dst, log, "ls: could not find directory \"%s\"", argv[1]);
        // log("ls: could not find directory \"%s\"", argv[1]);
    }

    return dst;
}

