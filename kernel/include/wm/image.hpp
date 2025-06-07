#pragma once
#include "element.hpp"
#include <cringe/vec.hpp>
#include <bmp.hpp>


namespace wm
{
    struct guiImage: public guiElement
    {
        uint8_t *m_pixels;
        int      m_bypp;
        int      m_pitch;
    
        guiImage()
        :   guiImage(nullptr, 0, 0, 0, 0) {  };

        guiImage( uint8_t *pixels, int width, int height, int bypp, int pitch )
        :   guiElement(ivec2(0, 0), ivec2(width, height)),
            m_pixels(pixels), m_bypp(bypp), m_pitch(pitch) {  };

        guiImage( void *bmp_addr );

        virtual void draw( guiFramebuffer& );

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

}
