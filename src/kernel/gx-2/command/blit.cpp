#ifndef __is_kernel
    #define __is_kernel
#endif

#include <type_traits>
#include <kernel/log.hpp>
#include <kpanic.h>

#include "../gx.hpp"
#include "../gx_assign.hpp"
#include "../gx_mix.hpp"



template <typename dst_type, typename src_type, bool depth_test, bool alpha_blend>
void gx_blit_loop( const gxDrawCmd &cmd )
{
    auto &bcmd = cmd.data.blit;
    // vec4 tint = cmd.conf.tint;
    float *depth = (float*)(gxGetTexture(cmd.conf.depth)->data);

    auto *dtex = gxGetTexture(bcmd.dst);
    auto *dst  = (dst_type*)(dtex->data);
    int   dstw = dtex->w;
    auto &dstrect = bcmd.dstrect;

    auto *stex = gxGetTexture(bcmd.src);
    auto *src  = (src_type*)(stex->data);
    int   srcw = stex->w;
    auto &srcrect = bcmd.srcrect;

    int xmin = std::max(dstrect.x, 0);
    int xmax = std::min(dstrect.x+dstrect.w, dtex->w);
    int ymin = std::max(dstrect.y, 0);
    int ymax = std::min(dstrect.y+dstrect.h, dtex->h);

    int srcx=0, srcy=0;
    int dstx=0, dsty=0;
    int dstidx=0, srcidx=0;
    float alpha = 0.0f;

    int jmax = ymax-ymin;
    int imax = xmax-xmin;
    int srcymin = srcrect.y;
    int srcymax = srcrect.y+srcrect.h;
    int srcxmin = srcrect.x;
    int srcxmax = srcrect.x+srcrect.w;

    for (int j=0; j<jmax; j++)
    {
        alpha = float(j) / float(jmax);
        srcy  = int((1.0f-alpha)*srcymin + alpha*srcymax + 0.5f);
        dsty  = ymin + j;

        for (int i=0; i<imax; i++)
        {
            alpha = float(i) / float(imax);
            srcx  = int((1.0f-alpha)*srcxmin + alpha*srcxmax + 0.5f);
            dstx  = xmin + i;

            dstidx = dstw*dsty + dstx;
            srcidx = srcw*srcy + srcx;

            if constexpr (depth_test == true)
            {
                if (depth[dstidx] < dstrect.z)
                    continue;
                depth[dstidx] = dstrect.z;
            }

            auto srcColor = src[srcidx];

            // if constexpr (std::is_same_v<src_type, u8vec4>)
            // {
            //     vec4 temp;
            //     gx_assign(temp, srcColor);
            //     temp *= tint;
            //     gx_assign(srcColor, temp);
            // }

            if constexpr (alpha_blend == true)
                gx_mix(dst[dstidx], srcColor);

            if constexpr (alpha_blend == false)
                gx_assign(dst[dstidx], srcColor);
        }
    }
}




template <typename dst_type, typename src_type>
static void blit_xx_yy( const gxDrawCmd &cmd )
{
    using fn_type = void (*)( const gxDrawCmd& );
    static fn_type table[4] = {
        gx_blit_loop<dst_type, src_type, false, false>,
        gx_blit_loop<dst_type, src_type, false, true>,
        gx_blit_loop<dst_type, src_type, true,  false>,
        gx_blit_loop<dst_type, src_type, true,  true>
    };

    int idx = 0;
    if (cmd.conf.blend) idx |= 1;
    if (cmd.conf.depth) idx |= 2;

    table[idx](cmd);
}


template <typename dst_type>
static void blit_xx( const gxDrawCmd &cmd )
{
    auto &bcmd = cmd.data.blit;
    auto *src  = gxGetTexture(bcmd.src);

    switch (src->format)
    {
        default: kpanic("[blit_xx] invalid src->format"); break;
        case GX_R8:      blit_xx_yy<dst_type, uint8_t>(cmd); break;
        case GX_RG8:     blit_xx_yy<dst_type, u8vec2>(cmd);  break;
        case GX_RGB8:    blit_xx_yy<dst_type, u8vec3>(cmd);  break;
        case GX_RGBA8:   blit_xx_yy<dst_type, u8vec4>(cmd);  break;
        case GX_RGBA32F: blit_xx_yy<dst_type, vec4>(cmd);    break;
    }
}


void gx_ExecCommand_Blit( const gxDrawCmd &cmd )
{
    auto &bcmd = cmd.data.blit;
    auto *dst = gxGetTexture(bcmd.dst);

    switch (dst->format)
    {
        default: kpanic("[gx_ExecCommand_Blit] invalid dst->format"); break;
        case GX_RG8:     blit_xx<u8vec2>(cmd); break;
        case GX_RGB8:    blit_xx<u8vec3>(cmd); break;
        case GX_RGBA8:   blit_xx<u8vec4>(cmd); break;
        case GX_RGBA32F: blit_xx<vec4>(cmd);   break;
    }

}

