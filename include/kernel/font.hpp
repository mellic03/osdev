#pragma once

#include <stddef.h>
#include <stdint.h>

#include <kernel/bmp.hpp>
#include <kernel/vec.hpp>


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
    uint32_t *m_data;

public:
    int W, H, SZ;
    const char *name;

    FontBuffer( const char *filename, ck_BMP_header* );
    // void init( const char *filename, ck_BMP_header* );

    idk::Bounds *getGlyph( char c );


    void *data()
    {
        return m_data;
    }

    const void *data() const
    {
        return m_data;
    }

    uint32_t *operator[] ( int i )
    {
        return &(m_data[W*i]);
    }

    const uint32_t *operator[] ( int i ) const
    {
        return &(m_data[W*i]);
    }

};

