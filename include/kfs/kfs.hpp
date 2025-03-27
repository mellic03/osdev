#pragma once
#include <kfile.h>

namespace KFS
{
    extern KFile *kstdio[3];
    extern KFile *kstderr;
    extern KFile *kstdin;
    extern KFile *kstdout;
    
    extern KFile *kdevio[16];
    extern KFile *kdevraw;
    extern KFile *kdevkey;
    
}

