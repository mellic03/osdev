#include "kshell.hpp"
#include "../ksystem.hpp"
#include <kstring.h>
#include <stdlib.h>
using namespace KShell;


extern idk::KSystem *sys;
extern vfsDirEntry *kshell_getdir( vfsDirEntry*, char* );
static uint64_t exec_args[16];


char *kshell_exec( char *dst, int argc, char **argv )
{
    auto &cwd = kshell_tty->getCWD();

    if (argc == 1)
    {
        dst = kssprintf(dst, "exec: argument required");
        return dst;
    }

    char *fname = argv[1];
    auto *file  = KFS::findFile(cwd, fname);

    if (!file)
    {
        return kssprintf(dst, "exec: could not find file \"%s\"", fname);
    }

    for (int i=2; i<argc; i++)
    {
        exec_args[i-2] = atoi(argv[i]);
    }

    uint64_t res = sys->execute(file->addr, file->size, 2, exec_args);

    return kssprintf(dst, "result: %d", res);
}

