#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>
#include <kernel/input.hpp>

namespace knl
{
    inline void (*panic)(const char*);
    inline void (*hcf)();
    inline ModuleInterface *(*findModule)(uint64_t, uint64_t);
}


namespace kinput
{
    struct InputMouseDevice
    {
        int x, y;
        int l, m, r;
    };
    
    inline void (*triggerMouseEvent)(uint32_t, uint32_t);
    inline void (*writeMsData)(const kinput::MsData*);
    inline void (*readMsData)(kinput::MsData*);
}

