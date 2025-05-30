#include <wm/render.hpp>
#include <cpu/cpu.hpp>
#include <kmemxx.hpp>


void wm::guiBlitImage( guiFramebuffer &dstimg, guiImage &srcimg, ivec2 dsttl )
{
    wm::guiBlitImage(dstimg, srcimg, dsttl, ivec2(0, 0), srcimg.m_sp);
}



void wm::guiBlitImage( guiFramebuffer &dstimg, guiImage &srcimg,
                       ivec2 dsttl, ivec2 srctl, ivec2 srcsp )
{
    uint8_t *dst = dstimg.m_pixels;
    uint8_t *src = srcimg.m_pixels;

    int dx = srcsp.x;
    int dy = srcsp.y;

    for (int i=0; i<dy; i++)
    {
        int dsty = (dsttl.y + i);
        int srcy = (srctl.y + i);

        if (std::clamp(dsty, 0, dstimg.m_sp.y) != dsty)
            continue;

        if (std::clamp(srcy, 0, srcimg.m_sp.y) != srcy)
            continue;

        // int dstidx = 4 * (dstimg.m_sp.x*dsty + dsttl.x % dstimg.m_sp.x);
        // int srcidx = 4 * (srcimg.m_sp.x*srcy + srctl.x % srcimg.m_sp.x);
        // movsq(dst+dstidx, src+srcidx, dx/2);

        for (int j=0; j<dx; j++)
        {
            int dstx = (dsttl.x + j);
            int srcx = (srctl.x + j);

            if (std::clamp(dstx, 0, dstimg.m_sp.x) != dstx)
            continue;

            if (std::clamp(srcx, 0, srcimg.m_sp.x) != srcx)
                continue;


            int dstidx = 4 * (dstimg.m_sp.x*dsty + dstx);
            int srcidx = 4 * (srcimg.m_sp.x*srcy + srcx);

            uint8_t a = src[srcidx+3];
            dst[dstidx+0] = ((255-a)*dst[dstidx+0] + a*src[srcidx+0]) / 255;
            dst[dstidx+1] = ((255-a)*dst[dstidx+1] + a*src[srcidx+1]) / 255;
            dst[dstidx+2] = ((255-a)*dst[dstidx+2] + a*src[srcidx+2]) / 255;
            dst[dstidx+3] = ((255-a)*dst[dstidx+3] + a*src[srcidx+3]) / 255;
        }
    }  
}

// extern "C"
// {
//     extern void cpu_STOSD( void *dst, uint32_t value, size_t count );
// }

static inline void stosd( void *dst, uint32_t value, size_t size )
{
    asm volatile("rep stosl" : "+D"(dst), "+c"(size), "+a"(value) : : "memory");
}

// static void guiBlitImageMovsq( guiFramebuffer &dstimg, guiImage *srcimg,
//                                ivec2 dsttl, ivec2 srctl, ivec2 srcsp )
// {
//     uint8_t *dst = dstimg.m_pixels;
//     uint8_t *src = srcimg.m_pixels;

//     int dx = srcsp.x;
//     int dy = srcsp.y;

//     int dstx = dsttl.x % dstimg.m_sp.x;
//     int srcx = srctl.x % srcimg.m_sp.x;

//     for (int i=0; i<dy; i++)
//     {
//         int dsty = (dsttl.y + i) % dstimg.m_sp.y;
//         int srcy = (srctl.y + i) % srcimg.m_sp.y;

//         int dstidx = 4 * (dstimg.m_sp.x*dsty + dstx);
//         int srcidx = 4 * (srcimg.m_sp.x*srcy + srcx);

//         movsq(dst+dstidx, src+srcidx, dx/2);
//     }  
// }


static void nextLine( const ivec2 &tl_bound,
                      int &x, int &y, const ivec2 &sp )
{
    int xmin = tl_bound.x;
    x = xmin;
    y += sp.y;
}


ivec2 wm::guiRenderFont( guiFramebuffer &dstimg, const ivec2 &tl_bound, const ivec2 &sp_bound,
                         guiFont &font, const char *str, ivec2 csr )
{
    ivec2 sp = font.getGlyphExtents();
    int   &x = csr.x;
    int   &y = csr.y;

    int xmin = tl_bound.x;
    int xmax = tl_bound.x + sp_bound.x;
    int ymin = tl_bound.y;
    int ymax = tl_bound.y + sp_bound.y;

    x = std::clamp(x, xmin, xmax);
    y = std::clamp(y, ymin, ymax);

    while (*str)
    {
        char ch = *(str++);

        // if (ch == '\n')
        //     nextLine(tl_bound, x, y, sp);

        if (x+sp.x >= xmax)
            nextLine(tl_bound, x, y, sp);
    
        if (y >= ymax)
            return ivec2(x, y);

        ivec2 tl = font.getGlyphCorner(ch);
        if (tl.x == -1)
            continue;

        guiBlitImage(dstimg, font, csr, tl, sp);

        x += sp.x;
    }

    return ivec2(x, y);
}

void wm::guiRenderHLine( guiFramebuffer &dstimg, const u8vec4 &color, int x0, int x1, int y )
{
    auto *dst = dstimg.m_pixels;
    uint8_t a = color.a;
    uint8_t ia = 255 - color.a;

    for (int x=x0; x<x1; x++)
    {
        int idx = dstimg.m_sp.x*4*y + 4*x;
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
        int idx = dstimg.m_sp.x*4*y + 4*x;
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

    for (int i=y; i<y+h; i++)
    {
        int idx = 4 * (dstimg.m_sp.x*i + x);
        stosd(dst+idx, U.dword, w);

        // for (int j=x; j<x+w; j++)
        // {
        //     int idx = dstimg.m_sp.x*4*i + 4*j;
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



