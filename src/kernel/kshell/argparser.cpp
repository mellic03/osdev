#include "kshell.hpp"
#include <kernel/log.hpp>
#include <kstring.h>
#include <string.h>

using namespace KShell;


int kshell_argparser( const char *src, char **argv )
{
    syslog log("kshell_argparser");
    int argc = 0;

    if (strlen(src) == 0)
    {
        return 0;
    }

    const char *end = src + strlen(src);

    char *token = argv[argc++];
    const char *A = skip_brk(src, " \n");
    const char *B = seek_brk(A, " \n");
    strncpy(token, A, B-A);
    A = B;
    log("token: \"%s\"", token);

    while (A < end && argc < MAX_ARG_COUNT)
    {
        token = argv[argc++];
        A = skip_brk(B, " \n");
        B = seek_brk(A, " \n");
        strncpy(token, A, B-A);
        log("token: \"%s\"", token);
        A = B;
    }

    return argc;
}



