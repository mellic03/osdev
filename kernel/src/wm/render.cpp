#include <wm/render.hpp>
#include <cpu/cpu.hpp>
#include <kmemxx.hpp>
#include <util/bitmanip.hpp>


void wm::guiBlitImage( guiFramebuffer &dstimg, guiImage &srcimg, ivec2 dsttl )
{
    wm::guiBlitImage(dstimg, srcimg, dsttl, ivec2(0, 0), srcimg.m_gsp);
}


void wm::guiBlitImage( guiFramebuffer &dstimg, guiImage &srcimg,
                       ivec2 dpos, ivec2 spos, ivec2 span )
{
    uint8_t *dst = dstimg.m_pixels;
    uint8_t *src = srcimg.m_pixels;

    ivec2 dstA = vec_clamp(dpos,      ivec2(0, 0), dstimg.m_gsp);
    ivec2 dstB = vec_clamp(dpos+span, ivec2(0, 0), dstimg.m_gsp);

    ivec2 srcA = vec_clamp(spos,      ivec2(0, 0), srcimg.m_gsp);
    ivec2 srcB = vec_clamp(spos+span, ivec2(0, 0), srcimg.m_gsp);

    int dx = std::min(dstB.x-dstA.x, srcB.x-srcA.x);
    int dy = std::min(dstB.y-dstA.y, srcB.y-srcA.y);

    // int dstpitch = dstimg.m_pitch;
    // int dstbypp  = dstimg.m_bypp;
    // int srcpitch = srcimg.m_pitch;
    // int srcbypp  = srcimg.m_bypp;

    int dsty = dstA.y;
    int srcy = srcA.y;

    for (int i=0; i<dy; i++)
    {
        // int dstidx = dstimg.m_pitch*dsty + dstimg.m_bypp*dstx;
        // int srcidx = srcimg.m_pitch*srcy + srcimg.m_bypp*srcx;
        // CPU::movs32(dst+dstidx, src+srcidx, dx);

        int dstx = dstA.x;
        int srcx = srcA.x;

        for (int j=0; j<dx; j++)
        {
            int dstidx = dstimg.m_pitch*dsty + dstimg.m_bypp*dstx;
            int srcidx = srcimg.m_pitch*srcy + srcimg.m_bypp*srcx;

            uint8_t a = src[srcidx+3];
        
            dst[dstidx+0] = ((255-a)*dst[dstidx+0] + a*src[srcidx+0]) / 255;
            dst[dstidx+1] = ((255-a)*dst[dstidx+1] + a*src[srcidx+1]) / 255;
            dst[dstidx+2] = ((255-a)*dst[dstidx+2] + a*src[srcidx+2]) / 255;
            dst[dstidx+3] = ((255-a)*dst[dstidx+3] + a*src[srcidx+3]) / 255;

            dstx++;
            srcx++;
        }
    
        dsty++;
        srcy++;
    }
}



static ivec2 nextLine( const ivec2 &csr, const ivec2 &tl_bound, const ivec2 &sp )
{
    int xmin = tl_bound.x;
    return ivec2(xmin, csr.y + sp.y);
}


ivec2 wm::guiRenderFont( guiFramebuffer &dstimg, const ivec2 &tl_bound, const ivec2 &sp_bound,
                         guiFont &font, const char *str, ivec2 csr )
{
    ivec2 sp = font.getGlyphExtents();
    // int   &x = csr.x;
    // int   &y = csr.y;

    int xmin = tl_bound.x;
    int xmax = tl_bound.x + sp_bound.x;
    int ymin = tl_bound.y;
    int ymax = tl_bound.y + sp_bound.y;

    csr.x = std::clamp(csr.x, xmin, xmax);
    csr.y = std::clamp(csr.y, ymin, ymax);

    while (*str)
    {
        char ch = *(str++);

        if (ch == '\n')
            csr = nextLine(csr, tl_bound, sp);

        if (csr.x + sp.x >= xmax)
            csr = nextLine(csr, tl_bound, sp);
    
        if (csr.y >= ymax)
            return csr;

        ivec2 tl = font.getGlyphCorner(ch);
        if (tl.x == -1)
            continue;

        guiBlitImage(dstimg, font, csr, tl, sp);

        csr.x += sp.x;
    }

    return csr;
}

void wm::guiRenderHLine( guiFramebuffer &dstimg, const u8vec4 &color, int x0, int x1, int y )
{
    auto *dst = dstimg.m_pixels;
    uint8_t a = color.a;
    uint8_t ia = 255 - color.a;

    for (int x=x0; x<x1; x++)
    {
        int idx = dstimg.m_pitch*y + 4*x;
        dst[idx+0] = (ia*dst[idx+0] + a*color[0]) / 255;
        dst[idx+1] = (ia*dst[idx+1] + a*color[1]) / 255;
        dst[idx+2] = (ia*dst[idx+2] + a*color[2]) / 255;
        dst[idx+3] = (ia*dst[idx+3] + a*color[3]) / 255;
    }
}


void wm::guiRenderVLine( guiFramebuffer &dstimg, const u8vec4 &color, int x, int y0, int y1 )
{
    auto *dst = dstimg.m_pixels;
    uint8_t a = color.a;
    uint8_t ia = 255 - color.a;

    for (int y=y0; y<y1; y++)
    {
        int idx = dstimg.m_pitch*y + 4*x;
        dst[idx+0] = (ia*dst[idx+0] + a*color[0]) / 255;
        dst[idx+1] = (ia*dst[idx+1] + a*color[1]) / 255;
        dst[idx+2] = (ia*dst[idx+2] + a*color[2]) / 255;
        dst[idx+3] = (ia*dst[idx+3] + a*color[3]) / 255;
    }
}


void wm::guiRenderRect( guiFramebuffer &dst, const u8vec4 &c, const ivec2 &tl, const ivec2 &sp )
{ wm::guiRenderRect(dst, c, tl.x, tl.y, sp.x, sp.y); }


void wm::guiRenderRect( guiFramebuffer &dstimg, const u8vec4 &color,
                        int x, int y, int w, int h )
{
    auto *dst = dstimg.m_pixels;
    // uint8_t a = color.a;
    // uint8_t ia = 255 - color.a;

    union RE {
        uint32_t dword;
        u8vec4   rgba;
        RE() { };
    } U; U.rgba = color;

    // uint64_t qword = (uint64_t(U.dword) << 32) + U.dword;

    for (int i=y; i<y+h; i++)
    {
        int idx = dstimg.m_pitch*i + dstimg.m_bypp*x;
        CPU::stos32(dst+idx, U.dword, w);
        // stosd(dst+idx, U.dword, w);

        // for (int j=x; j<x+w; j++)
        // {
        //     int idx = dstimg.m_pitch*i + dstimg.m_bypp*j;

        //     dst[idx+0] = (ia*dst[idx+0] + a*color[0]) / 255;
        //     dst[idx+1] = (ia*dst[idx+1] + a*color[1]) / 255;
        //     dst[idx+2] = (ia*dst[idx+2] + a*color[2]) / 255;
        //     dst[idx+3] = (ia*dst[idx+3] + a*color[3]) / 255;
        // }
    }
}


void wm::guiRenderBox( guiFramebuffer &dst, const u8vec4 &c, const ivec2 &tl, const ivec2 &sp )
{ wm::guiRenderBox(dst, c, tl.x, tl.y, sp.x, sp.y); }


void wm::guiRenderBox( guiFramebuffer &dst, const u8vec4 &color,
                       int x, int y, int w, int h )
{
    guiRenderHLine(dst, color, x, x+w-1, y);
    guiRenderHLine(dst, color, x, x+w-1, y+h-1);
    guiRenderVLine(dst, color, x, y, y+h-1);
    guiRenderVLine(dst, color, x+w-1, y, y+h-1);

}









// #include <math.h>

// void wm::guiRenderLine( guiFramebuffer &dst, const ivec2 &A, const ivec2 &B, const u8vec4 &color )
// {
//     vec2 start = vec2(vec_clamp(A, ivec2(0), dst.m_gsp));
//     vec2 end   = vec2(vec_clamp(B, ivec2(0), dst.m_gsp));
//     vec2 dir   = vec_normalize(end - start);

//     vec2 vRayUnitStepSize = {
//         fsqrt(1.0f + (dir.y / dir.x) * (dir.y / dir.x)),
//         fsqrt(1.0f + (dir.x / dir.y) * (dir.x / dir.y))
//     };

//     int row = int(start.y);
//     int col = int(start.x);

//     float dx = dir.x;
//     float dy = dir.y;

//     float dxdist = 0.0f;
//     float dydist = 0.0f;

//     vec2 vRayLength1D;
//     ivec2 vStep;

//     if (dx < 0)
//     {
//         vStep.x = -1;
//         dxdist = (start.x - col) * vRayUnitStepSize.x;
//     }

//     else
//     {
//         vStep.x = 1;
//         dxdist = (col+1 - start.x) * vRayUnitStepSize.x;
//     }

//     if (dy < 0)
//     {
//         vStep.y = -1;
//         dydist = (start.y - row) * vRayUnitStepSize.y;
//     }

//     else
//     {
//         vStep.y = 1;
//         dydist = (row+1 - start.y) * vRayUnitStepSize.y;
//     }

    
//     union {
//         uint32_t dword;
//         u8vec4   rgba;
//     } U{0}; U.rgba = color;

//     float ray_len = vec_length(end - start);
//     float dist = 0.0f;

//     while (dist < ray_len)
//     {
//         int idx = dst.m_pitch*row + dst.m_bypp*col;
//         dst.m_pixels[idx] = U.dword;

//         if (dxdist < dydist)
//         {
//             col += vStep.x;
//             dist = dxdist;
//             dxdist += vRayUnitStepSize.x;
//         }

//         else
//         {
//             row += vStep.y;
//             dist = dydist;
//             dydist += vRayUnitStepSize.y;
//         }
//     }

// }


