#pragma once
#include <kfile.h>

namespace KFS
{
    extern KFile *kstdio[3];
    extern KFile *kstderr;
    extern KFile *kstdin;
    extern KFile *kstdout;
    
    extern KFile *kdevio[2];
    extern KFile *kdevkey;
    extern KFile *kdevscn;
    
}

