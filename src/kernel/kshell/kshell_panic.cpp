#include "kshell.hpp"
#include <kpanic.h>


char *kshell_panic( char *dst, int, char** )
{
    kpanic("test panic");
    return dst;
}

