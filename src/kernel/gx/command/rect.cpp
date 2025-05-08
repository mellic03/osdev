#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kernel/log.hpp>
#include <kpanic.h>

#include "../gx.hpp"
#include "../gx_mix.hpp"


template <size_t N, typename dtype, bool alpha_blend, bool depth_test>
void gx_rect_loop( const gxDrawCmd &cmd )
{
    auto  &rcmd = cmd.data.rect;
    auto  *ctx = gxGetCurrent();
    float *depth = (float*)(gxGetTexture(ctx->target.depth)->data);

    auto *dtex = gxGetTexture(rcmd.dst);
    auto *dst  = (vec<N, dtype>*)(dtex->data);
    int   dstw = dtex->w;
    auto &rect = rcmd.rect;

    int xmin = std::max(rect.x, 0);
    int xmax = std::min(rect.x+rect.w, dtex->w-1);
    int ymin = std::max(rect.y, 0);
    int ymax = std::min(rect.y+rect.h, dtex->h-1);

    int dstidx=0;
    vec<N, dtype> srcColor;

    for (int dsty=ymin; dsty<ymax; dsty++)
    {
        int dst_y = std::clamp(dsty, 0, dtex->h-1);

        for (int dstx=xmin; dstx<xmax; dstx++)
        {
            int dst_x = std::clamp(dstx, 0, dtex->w-1);
            dstidx = dstw*dst_y + dst_x;

            if constexpr (depth_test == true)
            {
                if (depth[dstidx] < rect.z)
                    continue;
                depth[dstidx] = rect.z;
            }

            if constexpr (std::is_integral_v<dtype>)
                srcColor = vec<N, dtype>(rcmd.fillColor / 255.0f);
            else
                srcColor = vec<N, dtype>(rcmd.fillColor);

            if constexpr (alpha_blend && N==4)
                dst[dstidx] = gx_mix(dst[dstidx], srcColor);
            else
                dst[dstidx] = srcColor;
        }
    }
}


template <size_t N, typename dtype>
static void rect_xx( const gxDrawCmd &cmd )
{
    // gx_rect_loop<N, dtype, false, false>(cmd);

    using fn_type = void (*)( const gxDrawCmd& );
    static fn_type table[4] = {
        gx_rect_loop<N, dtype, false, false>,
        gx_rect_loop<N, dtype, false, true>,
        gx_rect_loop<N, dtype, true,  false>,
        gx_rect_loop<N, dtype, true,  true>
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
    auto *dst  = gxGetTexture(rcmd.dst);
    // syslog log("gx_ExecCommand_Rect: dst=%u", idx);

    switch (dst->format)
    {
        default: kpanic("[gx_ExecCommand_Rect] Invalid dst format"); break;
        // case GX_R8:      rect_xx<uint8_t>(cmd); break;
        case GX_RGB8:    rect_xx<3, u8vec3>(cmd);  break;
        case GX_RGBA8:   rect_xx<4, uint8_t>(cmd);  break;
        case GX_R32F:    rect_xx<1, float>(cmd);    break;
        // case GX_RGBA32F: rect_xx<vec4>(cmd);    break;
        // case GX_RGBA8: gx_rect_loop<u8vec4>(cmd); break;
        // case GX_R32F:    gx_rect_loop<float>(cmd);  break;
        // case GX_RGBA32F: gx_rect_loop<vec4>(cmd);   break;
    }

    // dispatch_table[idx](cmd);
}

