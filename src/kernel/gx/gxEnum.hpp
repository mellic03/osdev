#pragma once

enum gxEnum
{
    GX_UNSIGNED_BYTE = 1,
    GX_R8,      // +1
    GX_RG8,     // +2
    GX_RGB8,    // +3
    GX_RGBA8,   // +4 --> GX_UNSIGNED_BYTE+4 == GX_RGBA8 == u8vec4

    GX_FLOAT,
    GX_R32F,    // +1
    GX_RG32F,   // +2
    GX_RGB32F,  // +3
    GX_RGBA32F, // +4 --> GX_FLOAT+4 == GX_RGBA32F == vec4

    GX_BLEND,
    GX_DEPTH_TEST,

};

enum gxBitfield: uint32_t
{
    GX_COLOR_BUFFER_BIT   = 1<<1,
    GX_DEPTH_BUFFER_BIT   = 1<<2,
    GX_STENCIL_BUFFER_BIT = 1<<3
};


inline size_t gxGetFormatStride( uint32_t format )
{
    switch (format)
    {
        default:         return 0;
        case GX_R8:    return sizeof(uint8_t);
        case GX_RGB8:  return sizeof(u8vec3);
        case GX_RGBA8: return sizeof(u8vec4);
        case GX_R32F:    return sizeof(float);
        case GX_RGB32F:  return sizeof(vec3);
        case GX_RGBA32F: return sizeof(vec4);
    }
}

