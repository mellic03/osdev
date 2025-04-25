#include "sde.hpp"
#include "texture.hpp"
#include "../driver/mouse.hpp"
#include "../gx/gx.hpp"
#include "../kvideo/font.hpp"

#include <kthread.hpp>
#include <kmalloc.h>
#include <kpanic.h>
#include <stdio.h>
#include <algorithm>
#include <kernel/vfs.hpp>
#include <kernel/log.hpp>


sde::Frame     *sde::root;
sde::Frame     *sde::focus;

sde::MouseState sde::mouse;
ivec2          &sde::mpos = sde::mouse.pos;
int            &sde::mx   = sde::mpos.x;
int            &sde::my   = sde::mpos.y;

sde::Font      *sde::sysfont = nullptr;
vec4            sde::sysfont_tint = vec4(1.0f);

static void findFocus( sde::Frame* );
static bool rect_point_overlap( vec2 tl, vec2 sp, vec2 p );


static void onLMousePress()
{
    syslog::kprintf("[onLMousePress]\n");
    auto *&focus  = sde::focus;

    if (focus)
    {
        if (!rect_point_overlap(focus->m_wcorner, focus->m_span, sde::mpos))
            focus = nullptr;

        // else if (gxDepthSample(sde::mx, sde::my) >= 0.995f)
        //     sde::focus = nullptr;

        else if (sde::focus && sde::focus->_onLeftPress)
            sde::focus->_onLeftPress(sde::focus);
    }

    sde::mouse.ldown = true;
    sde::mouse.lup   = false;
}

static void onLMouseRelease()
{
    syslog::kprintf("[onLMouseRelease]\n");
    sde::mouse.ldown = false;
    sde::mouse.lup   = true;
}



void mouse_stuff()
{
    auto &mpos = hwdi_PS2Mouse::position;
    mpos.x = std::clamp(mpos.x, 1, kvideo::W-1);
    mpos.y = std::clamp(mpos.y, 1, kvideo::H-1);
    sde::mouse = {mpos, hwdi_PS2Mouse::left, hwdi_PS2Mouse::right};

    if (sde::mouse.ldown && !sde::focus)
    {
        findFocus(sde::root);
    }

    static ivec2 mprev = mpos;
    ivec2 mdelta = mpos - mprev;
    mprev = mpos;

    if (sde::focus && sde::mouse.ldown)
    {
        sde::focus->m_corner += mdelta;
        mdelta *= 0;
    }
}


void sde_main( void* )
{
    sde::root    = new sde::Frame(0, 0, kvideo::W, kvideo::H);
    sde::sysfont = new sde::Font("/font/cutive-w12hf18.bmp");

    int wp_w=0, wp_h=0, csr_w=0, csr_h=0;
    uint32_t wallpaper = sde::TextureLoad<uint8_t>("/img/undertale.bmp", &wp_w, &wp_h);
    uint32_t cursor    = sde::TextureLoad<uint8_t>("/img/cursor.bmp", &csr_w, &csr_h);

    hwdi_PS2Mouse::onLeftDown = onLMousePress;
    hwdi_PS2Mouse::onLeftUp   = onLMouseRelease;

    gxEnable(GX_BLEND);
    gxEnable(GX_DEPTH_TEST);

    while (true)
    {
        gxClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        gxClearDepth(1.0f);
        gxClear(GX_COLOR_BUFFER_BIT | GX_DEPTH_BUFFER_BIT);
        gxBlitTexture(wallpaper, {0, 0, wp_w, wp_h, 0.999f});
    
        for (auto *win: sde::root->m_children)
        {
            win->m_depth = 0.99f;
        
            if (win == sde::focus)
                win->m_depth = 0.5f;

            win->draw();
            kthread::yield();
        }

        for (auto *win: sde::root->m_children)
        {
            win->update();
        }

        mouse_stuff();
    
        gxDisable(GX_DEPTH_TEST);
        gxBlitTexture(cursor, {sde::mouse.pos.x, sde::mouse.pos.y, 2*csr_w, 2*csr_h, 0.01f});
        gxEnable(GX_DEPTH_TEST);

        gxFlush();
    }
}



sde::Frame*
sde::createWindow( ivec2 tl, ivec2 sp )
{
    auto *win = new sde::Frame(tl.x, tl.y, sp.x, sp.y);
    sde::root->m_children.push_back(win);
    return win;
}


void
sde::destroyWindow( sde::Frame *win )
{
    int idx = -1;

    for (int i=0; i<int(sde::root->m_children.size()); i++)
    {
        if (sde::root->m_children[i] == win)
        {
            idx = i;
            break;
        }
    }

    if (idx != -1)
    {
        sde::root->m_children[idx] = sde::root->m_children.back();
        sde::root->m_children.pop_back();
        delete win;
    }
}


// sde::WindowContext*
// sde::getCurrent()
// {
//     return sde_current;
// }


// void
// sde::makeCurrent( sde::WindowContext *ctx )
// {
//     sde_current = ctx;
// }


// void
// sde::hline( int x0, int x1, int y, const vec4 &color )
// {
//     auto *ctx = sde::getCurrent();
//     auto *tex = gxGetTexture(ctx->rgba);
//     auto *dst = (vec4*)(tex->data);
//     int width = tex->w;

//     if (y < 0 || y >= ctx->H)
//     {
//         return;
//     }

//     x0 = std::max(x0, 0);
//     x1 = std::min(x1, ctx->W-1);

//     for (int x=x0; x<=x1; x++)
//     {
//         dst[width*y + x] = color;
//     }
// }


// void
// sde::vline( int x, int y0, int y1, const vec4 &color )
// {
//     auto *ctx = sde::getCurrent();
//     auto *tex = gxGetTexture(ctx->rgba);
//     auto *dst = (vec4*)(tex->data);
//     int width = tex->w;

//     if (x < 0 || x >= ctx->W)
//     {
//         return;
//     }

//     y0 = std::max(y0, 0);
//     y1 = std::min(y1, ctx->H-1);

//     for (int y=y0; y<=y1; y++)
//     {
//         dst[width*y + x] = color;
//     }
// }



template <typename T>
T idk_mix( const T &x, const T&y, float a )
{
    return (1.0f-a)*x + a*y;
}


vec4 unpack_vec4( uint32_t src )
{
    float a = float((src >> 24) & 0xFF) / 255.0f;
    float r = float((src >> 16) & 0xFF) / 255.0f;
    float g = float((src >> 8) & 0xFF)  / 255.0f;
    float b = float((src & 0xFF))       / 255.0f;

    return vec4(r, g, b, a);
}




void sde::sysfont_putchar( char ch, int x, int y, float z )
{
    ivec2 span   = sysfont->getGlyphExtents();
    ivec2 corner = sysfont->getGlyphCorner(ch);

    if (corner.x < 0)
    {
        return;
    }
  
    gxBlitTexture(
        sysfont->m_tex,
        {x, y, span.x, span.y, z},
        {corner.x, corner.y, span.x, span.y}
    );
}






static bool rect_point_overlap( vec2 tl, vec2 sp, vec2 p )
{
    bool x = (tl.x <= p.x) && (p.x <= tl.x + sp.x);
    bool y = (tl.y <= p.y) && (p.y <= tl.y + sp.y);
    return x && y; 
}


static void findFocus( sde::Frame *curr )
{
    static float zNearest = 1.0f;
    if (curr == sde::root)
    {
        zNearest = 1.0f;
    }

    else
    {
        auto  &mouse   = sde::mouse;
        bool  overlap  = rect_point_overlap(curr->m_wcorner, curr->m_span, vec2(mouse.pos));
        float zCurrent = gxDepthSample(mouse.pos.x, mouse.pos.y);

        if (overlap && (zCurrent < zNearest))
        {
            zNearest = zCurrent;
            sde::focus = curr;
        }
    }

    for (auto *F: curr->m_children)
    {
        findFocus(F);
    }
}
