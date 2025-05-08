#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kernel/log.hpp>
#include <kpanic.h>

#include "../gx.hpp"
#include "../gx_assign.hpp"
#include "../gx_mix.hpp"


template <typename dst_type, bool depth_test, bool alpha_blend>
void gx_rect_loop( const gxDrawCmd &cmd )
{
    auto  &rcmd = cmd.data.rect;
    auto  *ctx = gxGetCurrent();
    float *depth = (float*)(gxGetTexture(ctx->target.depth)->data);

    auto *dtex = gxGetTexture(rcmd.dst);
    auto *dst  = (dst_type*)(dtex->data);
    int   dstw = dtex->w;
    auto &rect = rcmd.rect;

    int xmin = std::max(rect.x, 0);
    int xmax = std::min(rect.x+rect.w, dtex->w-1);
    int ymin = std::max(rect.y, 0);
    int ymax = std::min(rect.y+rect.h, dtex->h-1);

    // int xmin = std::clamp(rect.x, 0, dtex->w-1);
    // int xmax = std::clamp(rect.x+rect.w, 0, dtex->w-1);
    // int ymin = std::clamp(rect.y, 0, dtex->h-1);
    // int ymax = std::clamp(rect.y+rect.h, 0, dtex->h-1);
    int dstidx=0;

    for (int dsty=ymin; dsty<ymax; dsty++)
    {
        for (int dstx=xmin; dstx<xmax; dstx++)
        {
            dstidx = dstw*dsty + dstx;

            if constexpr (depth_test == true)
            {
                if (depth[dstidx] < rect.z)
                    continue;
                depth[dstidx] = rect.z;
            }

            if constexpr (alpha_blend == true)
            {
                gx_mix(dst[dstidx], rcmd.fillColor);
            }
        
            else if constexpr (alpha_blend == false)
            {
                gx_assign(dst[dstidx], rcmd.fillColor);
            }
        }
    }
}


template <typename dst_type>
static void rect_xx( const gxDrawCmd &cmd )
{
    using fn_type = void (*)( const gxDrawCmd& );
    static fn_type table[4] = {
        gx_rect_loop<dst_type, false, false>,
        gx_rect_loop<dst_type, false, true>,
        gx_rect_loop<dst_type, true,  false>,
        gx_rect_loop<dst_type, true,  true>
    };

    int idx = 0;
    if (cmd.conf.blend) idx |= 1;
    if (cmd.conf.depth) idx |= 2;

    table[idx](cmd);
}


void gx_ExecCommand_Rect( const gxDrawCmd &cmd )
{
    // auto &cmd = cmd0.data.rect
    auto &rcmd = cmd.data.rect;
    auto idx  = gxGetTexture(rcmd.dst)->format;

    switch (idx)
    {
        default: kpanic("[gx_ExecCommand_Rect] Invalid format"); break;
        case GX_R8:      rect_xx<uint8_t>(cmd); break;
        case GX_RG8:     rect_xx<u8vec2>(cmd);  break;
        case GX_RGB8:    rect_xx<u8vec3>(cmd);  break;
        case GX_RGBA8:   rect_xx<u8vec4>(cmd);  break;
        case GX_R32F:    rect_xx<float>(cmd);   break;
        case GX_RGBA32F: rect_xx<vec4>(cmd);    break;
        // case GX_RGBA8: gx_rect_loop<u8vec4>(cmd); break;
        // case GX_R32F:    gx_rect_loop<float>(cmd);  break;
        // case GX_RGBA32F: gx_rect_loop<vec4>(cmd);   break;
    }

    // dispatch_table[idx](cmd);
}

