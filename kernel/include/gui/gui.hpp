#pragma once
#include <cringe/vec.hpp>
#include "container.hpp"
#include "image.hpp"


void guiBlitImage( guiFramebuffer *dstimg, guiImage *srcimg, ivec2 dsttl,
                   ivec2 srctl, ivec2 srcsp );
// const guiContainer &dstrect,
//                    const guiContainer &srcrect );

void guiSetFont( guiFont* );
ivec2 guiRenderFont( guiFramebuffer *dstimg, const guiContainer &dstb,
                     const char *str, ivec2 csr );

void guiRenderHLine( guiFramebuffer *dst, const u8vec4 &color, int x0, int x1, int y );
void guiRenderVLine( guiFramebuffer *dst, const u8vec4 &color, int x, int y0, int y1 );
void guiRenderRect ( guiFramebuffer *dst, const u8vec4 &color, int x, int y, int w, int h );



