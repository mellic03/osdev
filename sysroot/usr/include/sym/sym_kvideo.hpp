#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>

namespace kvideo
{
    inline int W;
    inline int H;
    inline uint8_t *frontbuffer;
    inline uint8_t *backbuffer;
    void (*clearBuffer)(uint8_t *buffer);
    void (*fillColor)  (uint8_t, uint8_t, uint8_t, uint8_t);
    void (*fillBuffer) (uint8_t *buffer);
    void (*rect)       (int, int, int, int);
    void (*swapBuffers)();
}

