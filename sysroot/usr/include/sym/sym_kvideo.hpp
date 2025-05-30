#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>
#include <cringe/vec.hpp>

namespace kvideo
{
    inline ivec2 *CSR;
    inline int W;
    inline int H;
    inline int pitch;
    inline size_t BPP;
    inline uint8_t *frontbuffer;
    inline uint8_t *backbuffer;
    void (*swapBuffers)();
}

