#pragma once
#include "ck_file.h"

namespace ck
{
    static constexpr uint32_t FILE_MAGIC = 0x0AFACAD0;

    enum FileFlag_: uint32_t
    {
        FileFlag_BUF    = 1 << 1,
        FileFlag_MMAP   = 1 << 2,
        FileFlag_STR    = 1 << 3,
        FileFlag_OWNING = 1 << 4,
    
        FileFlag_STDIO  = 1 << 16,
        FileFlag_STDIN  = 1 << 17 | FileFlag_STDIO | FileFlag_MMAP,
        FileFlag_STDOUT = 1 << 18 | FileFlag_STDIO | FileFlag_MMAP,
        FileFlag_STDERR = 1 << 19 | FileFlag_STDIO | FileFlag_MMAP,
    };
    
    enum FileStatus_: uint32_t
    {
        FileStatus_GOOD  =  0,
        FileStatus_DIRTY =  1,
        FileStatus_OTHER =  2
    };
}


