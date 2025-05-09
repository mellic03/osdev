#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>

namespace kvideo
{
    inline int W;
    inline int H;
    inline uint8_t *frontbuffer;
    inline uint8_t *backbuffer;
    inline void (*rect)(int, int, int, int);
    inline void (*fill)(uint8_t, uint8_t, uint8_t, uint8_t);
    inline void (*swapBuffers)();
}

