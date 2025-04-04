#pragma once

#include <stdint.h>
#include <kdef.h>
#include <kfile.h>


namespace KFS
{
    extern KFile *kstdio[3];
    extern KFile *kstderr;
    extern KFile *kstdin;
    extern KFile *kstdout;

}


