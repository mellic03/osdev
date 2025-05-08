#include "kshell.hpp"
#include <kpanic.h>


char *kshell_panic( char *dst, int, char[16][32] )
{
    kpanic("test panic");
    return dst;
}

