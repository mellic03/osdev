#pragma once
#include <kmath/vec.hpp>
#include "gxEnum.hpp"
#include "gxResource.hpp"




template <typename dtype, uint32_t blend_mode, bool depth_test>
struct gxTextureWrite
{
    // using value_type = dtype;
    // using blend_type = blend_mode;
    // using depth_type = depth_test;

    dtype *pixels;

    gxTextureWrite(void *data )
    :   pixels(data)
    {

    };
    
};



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


