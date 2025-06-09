#pragma once
#include <stdint.h>
#include <driver/video.hpp>

namespace SVGA
{
    void     init();
    bool     enabled();
    void     setMode( uint32_t w, uint32_t h, uint32_t bpp );
    void     videoFlush( uint32_t streamId );
    void     update( uint32_t x, uint32_t y, uint32_t w, uint32_t h );

    void     writeReg( uint32_t idx, uint32_t value );
    uint32_t readReg( uint32_t idx );

    void     swapBuffers();

    knl::VideoInterface *getInterface();
}



struct SVGADevice
{
    uint32_t   ioBase;
    uint32_t  *fifoMem;
    uint8_t   *fbMem;
    uint32_t   fifoSize;
    uint32_t   fbSize;
    uint32_t   vramSize;
 
    uint32_t   deviceVersionId;
    uint32_t   capabilities;
 
    uint32_t   width;
    uint32_t   height;
    uint32_t   bpp;
    uint32_t   pitch;
 
    struct {
       uint32_t  reservedSize;
       bool      usingBounceBuffer;
       uint8_t   bounceBuffer[1024 * 1024];
       uint32_t  nextFence;
    } fifo;
 
    // volatile struct {
    //    uint32_t        pending;
    //    uint32_t        switchContext;
    //    IntrContext   oldContext;
    //    IntrContext   newContext;
    //    uint32_t        count;
    // } irq;

};