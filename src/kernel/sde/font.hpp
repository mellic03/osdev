#pragma once
#include <kmath/vec.hpp>
#include <kfs/bmp.hpp>
#include "texture.hpp"


namespace sde
{
    class Font
    {
    private:

    public:
        uint32_t m_tex;
        int W, H, SZ;
        vec2 glyph_scale;

        Font();
        Font( uint32_t tex, int w, int h );
        Font( const char *filepath );

        ivec2 getGlyphExtents();
        ivec2 getGlyphCorner( char c );
    };

}