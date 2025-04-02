#include "kshell.hpp"
#include <kstring.h>
using namespace KShell;
extern kTTY *kshell_tty;


extern vfsDirEntry *kshell_getdir( vfsDirEntry*, char* );


// static char *internal_ls( char *dst, vfsDirEntry *dir, int max_depth )
// {
//     for (auto *E: dir->children)
//     {
//         if (E->is_dir())
//             dst = kssprintln(dst, "%s", E->name);
//     }

//     for (auto *E: dir->children)
//     {
//         if (E->is_file())
//             dst = kssprintln(dst, "%s", E->name);
//     }

//     return dst;
// }


char*
internal_ls( char *dst, syslog &log, vfsDirEntry *dir, int depth, int max_depth )
{
    if (depth >= max_depth)
        return dst;
    KShell::pushIndent();

    for (auto *E: dir->children)
    {
        if (E->is_dir())
        {
            dst = kssprintln(dst, log, "%s/", E->name);
            dst = internal_ls(dst, log, (vfsDirEntry*)E, depth+1, max_depth);
        }
    }

    for (auto *E: dir->children)
    {
        if (E->is_file())
        {
           dst = kssprintln(dst, "%s", E->name);
        }
    }

    KShell::popIndent();
    return dst;
}


#include <ipc.hpp>


char *kshell_ls( char *dst, int argc, char **argv )
{
    syslog log("kshell_ls");
    auto &cwd = kshell_tty->getCWD();
    auto *dir = (argc==1) ? cwd : vfsFindDirectory(cwd, argv[1]);

    if (argc==2 && strcmp(argv[1], "--R")==0)
        return internal_ls(dst, log, cwd, 0, 16);

    if (dir && argc==3 && strcmp(argv[2], "--R")==0)
        return internal_ls(dst, log, dir, 0, 16);

    if (dir) dst = internal_ls(dst, log, dir, 0, 1);
    else     dst = kssprintln(dst, "ls: could not find directory \"%s\"", argv[1]);

    return dst;
}


