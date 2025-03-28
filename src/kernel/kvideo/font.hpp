#pragma once

#include <stddef.h>
#include <stdint.h>

#include <kfs/bmp.hpp>
#include <kmath/vec.hpp>


namespace idk
{
    struct Bounds
    {
        ivec2 corner;
        ivec2 extents;
    };
}


namespace idk
{
    class FontBuffer;
}



class idk::FontBuffer
{
private:
    vec4 *m_data;

public:
    const char *name;
    int W, H, SZ;
    vec2 glyph_scale;

    FontBuffer();
    FontBuffer( const char *filename, ck_BMP_header* );
    // void init( const char *filename, ck_BMP_header* );

    ivec2 getGlyphExtents();
    ivec2 getGlyphCorner( char c );


    void *data()
    {
        return m_data;
    }

    const void *data() const
    {
        return m_data;
    }

    vec4 *operator[] ( int i )
    {
        return &(m_data[W*i]);
    }

    const vec4 *operator[] ( int i ) const
    {
        return &(m_data[W*i]);
    }

};

