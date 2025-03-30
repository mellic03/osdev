#include "kshell.hpp"
#include <kpanic.h>


char *kshell_panic( char *dst, int argc, char **argv )
{
    kpanic("test panic");

    return dst;
}

