#pragma once
#include <cringe/vec.hpp>



class Comp_ImageBase
{
private:

public:
    uint8_t *pixels;
    int w, h;


};



class Comp_Framebuffer: public Comp_ImageBase
{
private:

public:

    void blit( const ivec2 &dst_tl, Comp_ImageBase *src,
               const ivec2 &src_tl, const ivec2 &src_sp );


};


void Comp_Framebuffer::blit( const ivec2 &dst_tl, Comp_ImageBase *img,
                             const ivec2 &src_tl, const ivec2 &src_sp )
{
    uint8_t *dst = this->pixels;
    uint8_t *src = img->pixels;

    for (int i=0; i<src_sp.y; i++)
    {
        int dsty = (dst_tl.y + i) % this->h;
        int srcy = (src_tl.y + i) % img->h;

        for (int j=0; j<src_sp.x; j++)
        {
            int dstx = (dst_tl.x + j) % this->w;
            int srcx = (src_tl.x + j) % img->w;

            int dstidx = 4 * (this->w*dsty + dstx);
            int srcidx = 4 * (img->w*srcy + srcx);

            uint8_t alpha = src[srcidx+3];
            dst[dstidx+0] = ((255-alpha)*dst[dstidx+0] + alpha*src[srcidx+0]) / 255;
            dst[dstidx+1] = ((255-alpha)*dst[dstidx+1] + alpha*src[srcidx+1]) / 255;
            dst[dstidx+2] = ((255-alpha)*dst[dstidx+2] + alpha*src[srcidx+2]) / 255;
            dst[dstidx+3] = ((255-alpha)*dst[dstidx+3] + alpha*src[srcidx+3]) / 255;

            // for (int k=0; k<4; k++)
            // {
            //     dst[dstidx+k] = ((255-alpha)*dst[dstidx+k] + alpha*src[srcidx+k]) / 255;
            // }
        }
    }  
}


