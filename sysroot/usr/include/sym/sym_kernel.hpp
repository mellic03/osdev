#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>

namespace kernel
{
    inline void (*panic)(const char*);
    inline void (*hang)();
    inline ModuleInterface *(*findModule)(uint64_t, uint64_t);
}

