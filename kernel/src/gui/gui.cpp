#include <gui/gui.hpp>
#include <kmemxx.hpp>



void guiBlitImage( guiFramebuffer *dstimg, guiImage *srcimg,
                   ivec2 dsttl, ivec2 srctl, ivec2 srcsp )
{
    uint8_t *dst = dstimg->pixels;
    uint8_t *src = srcimg->pixels;

    int dx = srcsp.x;
    int dy = srcsp.y;

    for (int i=0; i<dy; i++)
    {
        int dsty = (dsttl.y + i) % dstimg->h;
        int srcy = (srctl.y + i) % srcimg->h;

        for (int j=0; j<dx; j++)
        {
            int dstx = (dsttl.x + j) % dstimg->w;
            int srcx = (srctl.x + j) % srcimg->w;

            int dstidx = 4 * (dstimg->w*dsty + dstx);
            int srcidx = 4 * (srcimg->w*srcy + srcx);

            uint8_t alpha = src[srcidx+3];
            dst[dstidx+0] = ((255-alpha)*dst[dstidx+0] + alpha*src[srcidx+0]) / 255;
            dst[dstidx+1] = ((255-alpha)*dst[dstidx+1] + alpha*src[srcidx+1]) / 255;
            dst[dstidx+2] = ((255-alpha)*dst[dstidx+2] + alpha*src[srcidx+2]) / 255;
            dst[dstidx+3] = ((255-alpha)*dst[dstidx+3] + alpha*src[srcidx+3]) / 255;

            // for (int k=0; k<4; k++)
            // {
            //     dst[dstidx+k] = ((255-alpha)*dst[dstidx+k] + alpha*src[srcidx+k]) / 255;
            // }
        }
    }  
}



static void nextLine( const guiContainer &bounds, int &x, int &y,
                      const ivec2 &sp )
{
    int xmin = bounds.xmin();
    int ymin = bounds.ymin();
    int ymax = bounds.ymax();

    x = xmin;
    y += sp.y;

    if (y >= ymax)
        y = ymin;
}


static guiFont *guiDefaultFont = nullptr;

void guiSetFont( guiFont *font )
{
    guiDefaultFont = font;
}


ivec2 guiRenderFont( guiFramebuffer *dstimg, const guiContainer &dstb,
                     const char *str, ivec2 csr )
{
    if (!guiDefaultFont)
    {
        return ivec2(-1);
    }

    auto *font = guiDefaultFont;
    ivec2 sp = font->getGlyphExtents();
    int   &x = csr.x;
    int   &y = csr.y;

    int xmin = dstb.xmin();
    int xmax = dstb.xmax();
    int ymin = dstb.ymin();
    int ymax = dstb.ymax();

    x = std::clamp(x, xmin, xmax);
    y = std::clamp(y, ymin, ymax);

    while (*str)
    {
        char ch = *(str++);

        if (ch == '\n')
            nextLine(dstb, x, y, sp);

        if (x+sp.x >= xmax)
            nextLine(dstb, x, y, sp);

        ivec2 tl = font->getGlyphCorner(ch);
        if (tl.x == -1)
            continue;

        guiBlitImage(dstimg, font, csr, tl, sp);

        x += sp.x;
    }

    return ivec2(x, y);
}


void guiRenderHLine( guiFramebuffer *dstimg, const u8vec4 &color, int x0, int x1, int y )
{
    auto *dst = dstimg->pixels;
    uint8_t a = color.a;
    uint8_t ia = 255 - color.a;

    for (int x=x0; x<x1; x++)
    {
        int idx = dstimg->w*4*y + 4*x;
        dst[idx+0] = (ia*dst[idx+0] + a*color[0]) / 255;
        dst[idx+1] = (ia*dst[idx+1] + a*color[1]) / 255;
        dst[idx+2] = (ia*dst[idx+2] + a*color[2]) / 255;
        dst[idx+3] = (ia*dst[idx+3] + a*color[3]) / 255;
    }
}


void guiRenderVLine( guiFramebuffer *dstimg, const u8vec4 &color, int x, int y0, int y1 )
{
    auto *dst = dstimg->pixels;
    uint8_t a = color.a;
    uint8_t ia = 255 - color.a;

    for (int y=y0; y<y1; y++)
    {
        int idx = dstimg->w*4*y + 4*x;
        dst[idx+0] = (ia*dst[idx+0] + a*color[0]) / 255;
        dst[idx+1] = (ia*dst[idx+1] + a*color[1]) / 255;
        dst[idx+2] = (ia*dst[idx+2] + a*color[2]) / 255;
        dst[idx+3] = (ia*dst[idx+3] + a*color[3]) / 255;
    }
}


void guiRenderRect( guiFramebuffer *dstimg, const u8vec4 &color, int x, int y, int w, int h )
{
    auto *dst = dstimg->pixels;
    uint8_t a = color.a;
    uint8_t ia = 255 - color.a;

    for (int i=y; i<y+h; i++)
    {
        for (int j=x; j<x+w; j++)
        {
            int idx = dstimg->w*4*i + 4*j;
            dst[idx+0] = (ia*dst[idx+0] + a*color[0]) / 255;
            dst[idx+1] = (ia*dst[idx+1] + a*color[1]) / 255;
            dst[idx+2] = (ia*dst[idx+2] + a*color[2]) / 255;
            dst[idx+3] = (ia*dst[idx+3] + a*color[3]) / 255;
        }
    }
}



