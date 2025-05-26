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
    // void (*clearBuffer)(uint8_t *buffer);
    // void (*fillColor)  (uint8_t, uint8_t, uint8_t, uint8_t);
    // void (*fillBuffer) (uint8_t *buffer);
    // void (*rect)       (int, int, int, int);
    void (*blit)(const ivec2 &dsttl, uint8_t *img, int imgw, int imgh,
                        const ivec2 &srctl, const ivec2 &srcsp);
    // ivec2 (*renderString)(const ivec4&, const char*, const ivec2&);
    // void (*cursorString)(const char*);
    void (*swapBuffers)();
}

