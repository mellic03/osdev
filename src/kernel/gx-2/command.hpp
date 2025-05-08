#pragma once
#include <kmath/vec.hpp>
#include "gxContext.hpp"
#include "gxTexture.hpp"
#include "gxPrimitive.hpp"

enum gxDrawCmd_: uint32_t
{
    gxDrawCmd_Blit = 1,
    gxDrawCmd_Line,
    gxDrawCmd_Rect,
    gxDrawCmd_Tris,
    gxDrawCmd_Swap,
    gxDrawCmd_MAX
};

struct gxDrawCmdBlit
{
    uint32_t dst, src;
    gxRect dstrect, srcrect;
};

struct gxDrawCmdLine
{
    uint32_t dst;
    vec2 start;
    vec2 end;
    vec4 color;
};

struct gxDrawCmdRect
{
    uint32_t dst;
    gxRect   rect;
    bool     fill;
    int      border;
    vec4     fillColor;
    vec4     borderColor;
};

struct gxDrawCmdTris
{
    uint32_t dst, src;
    size_t count;
    size_t offset;
};

struct gxDrawCmd
{
    gxDrawCmd_ type;
    gx_Context::Conf conf;

    union union_type
    {
        gxDrawCmdBlit blit;
        gxDrawCmdLine line;
        gxDrawCmdRect rect;
        gxDrawCmdTris tris;
        union_type() {  };
    } data;

} __attribute__((aligned(8)));


void gx_ExecCommand( const gxDrawCmd &cmd );

