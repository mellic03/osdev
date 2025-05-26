#pragma once
#include <cringe/vec.hpp>


struct guiImage
{
    uint8_t *pixels;
    int w, h, bpp;

    guiImage( uint8_t *p, int width, int height, int bitspp )
    :   pixels(p), w(width), h(height), bpp(bitspp) {  };

    guiImage()
    :   guiImage(nullptr, 0, 0, 0) {  };

};


struct guiFont: public guiImage
{
    vec2 glyph_scale;
    using guiImage::guiImage;

    ivec2 getGlyphExtents();
    ivec2 getGlyphCorner( char c );
};


struct guiFramebuffer: public guiImage
{
    using guiImage::guiImage;

};

