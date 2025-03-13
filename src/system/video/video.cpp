#include "video.hpp"


static limine_framebuffer fb;


void
fb_init( limine_framebuffer &__fb__ )
{
    fb = __fb__;
}


void
fb_rect( const ck_Rect &rect, const ck_Color &color )
{
    uint32_t *pixels = (uint32_t*)(fb.address);

    int xmin = rect.x;
    int xmax = rect.x + rect.w;
    int ymin = rect.y;
    int ymax = rect.y + rect.h;

    uint32_t rgba = (uint32_t(color.r) << fb.red_mask_shift)
                  + (uint32_t(color.g) << fb.green_mask_shift)
                  + (uint32_t(color.b) << fb.blue_mask_shift)
                  + (color.a);

    for (int y=ymin; y<ymax; y++)
    {
        for (int x=xmin; x<xmax; x++)
        {
            pixels[y*fb.width + x] = rgba;
        }
    }
}

