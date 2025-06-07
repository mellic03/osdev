#pragma once
#include <cringe/vec.hpp>
#include "image.hpp"


namespace wm
{
    void guiBlitImage( guiFramebuffer &dstimg, guiImage &srcimg, ivec2 dsttl );

    void guiBlitImage( guiFramebuffer &dstimg, guiImage &srcimg, ivec2 dsttl,
                       ivec2 srctl, ivec2 srcsp );

    // void guiSetFont( guiFont* );
    ivec2 guiRenderFont( guiFramebuffer &dstimg, const ivec2 &tl_bound, const ivec2 &sp_bound,
                         guiFont&, const char *str, ivec2 csr );

    void guiRenderHLine( guiFramebuffer &dst, const u8vec4 &color, int x0, int x1, int y );
    void guiRenderVLine( guiFramebuffer &dst, const u8vec4 &color, int x, int y0, int y1 );


    void guiRenderRect ( guiFramebuffer&, const u8vec4 &color, const ivec2 &tl, const ivec2 &sp );
    void guiRenderRect ( guiFramebuffer &dst, const u8vec4 &color, int x, int y, int w, int h );

    void guiRenderBox  ( guiFramebuffer&, const u8vec4 &color, const ivec2 &tl, const ivec2 &sp );
    void guiRenderBox  ( guiFramebuffer&st, const u8vec4 &color, int x, int y, int w, int h );

    // void guiRenderLine( guiFramebuffer&, const ivec2 &A, const ivec2 &B, const u8vec4 &color );

}

