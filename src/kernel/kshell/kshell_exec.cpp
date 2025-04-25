// #include "kshell.hpp"
// #include "../ksystem.hpp"
// #include <kstring.h>
// #include <stdlib.h>
// using namespace KShell;


// extern vfsDirEntry *kshell_getdir( vfsDirEntry*, char* );
// static uint64_t exec_args[16];


// char *kshell_exec( char *dst, int argc, char argv[16][32]  )
// {
//     auto &cwd = kshell_tty->getCWD();

//     if (argc == 1)
//     {
//         dst = kssprintln(dst, "exec: argument required");
//         return dst;
//     }

//     char *fname = argv[1];
//     auto *file  = vfsFindFile(cwd, fname);

//     if (!file)
//     {
//         return kssprintln(dst, "exec: could not find file \"%s\"", fname);
//     }

//     for (int i=2; i<argc; i++)
//     {
//         exec_args[i-2] = atoi(argv[i]);
//     }

//     int res = sys->execute(file->addr, file->size, 2, exec_args);

//     return kssprintln(dst, "result: %d", res);
// }

