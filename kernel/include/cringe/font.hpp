#pragma once
#include <cringe/vec.hpp>


namespace cringe
{
    class Font
    {
    private:

    public:
        uint8_t *m_img;
        int W, H, SZ;
        vec2 glyph_scale;

        Font();
        Font( uint8_t *img, int w, int h );
        // Font( const char *filepath );

        ivec2 getGlyphExtents();
        ivec2 getGlyphCorner( char c );
    };

    // void renderGlyph( char ch, int x, int y, uint8_t *img, int imgw, int imgh );
}

