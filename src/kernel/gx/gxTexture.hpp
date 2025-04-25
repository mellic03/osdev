#pragma once
#include <kmath/vec.hpp>
#include "gxEnum.hpp"
#include "gxResource.hpp"

struct gxTexture
{ 
    gxResourceStatus_ status;
    gxResourceFlag_   flags;
    uint32_t format;
    size_t   stride;
    void    *data;
    int      w, h;
};

struct gxRect
{
    int x, y, w, h;
    float z = 0.99f;
};


uint32_t   gxCreateTextureExplicit( uint32_t flags, uint32_t format, void *data, int w, int h );
uint32_t   gxCreateTexture( uint32_t format, void *data, int w, int h );
void       gxDeleteTexture( uint32_t );
gxTexture *gxGetTexture( uint32_t );

