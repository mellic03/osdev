#pragma once

#include <stddef.h>
#include <stdint.h>
#include "system/file/bmp.hpp"
#include "./vec.hpp"


namespace idk
{
    struct Bounds
    {
        ivec2 corner;
        ivec2 extents;
    };
}


namespace idk::video
{
    class Buffer;
    class FontBuffer;
}


class idk::video::Buffer
{
protected:
    uint32_t *m_data;

public:
    int W, H, SZ;

    Buffer();
    Buffer( int w, int h );
    Buffer( int w, int h, uint32_t *data );
    virtual ~Buffer();


    void *data() { return m_data; }
    const void *data() const { return m_data; }

    uint32_t *operator[] ( int i )
    {
        return &(m_data[W*i]);
    }

    const uint32_t *operator[] ( int i ) const
    {
        return &(m_data[W*i]);
    }
};


class idk::video::FontBuffer: public video::Buffer
{
public:
    const char *name;

    FontBuffer( const char *filename, ck_BMP_header* );
    ~FontBuffer() {  };
    // void init( const char *filename, ck_BMP_header* );

    /**
     * Returns {xmin, xmax, ymin, ymax}
     */
    idk::Bounds getGlyph( char c );

};

