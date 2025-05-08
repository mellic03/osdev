#ifndef __is_kernel
    #define __is_kernel
#endif

#include <type_traits>
#include <kernel/log.hpp>
#include <kpanic.h>

#include "../gx.hpp"
#include "../gx_assign.hpp"
#include "../gx_mix.hpp"

// template <typename dst_type, typename src_type>
// void gx_blit_loop( const gxDrawCmd& )
// {
//     static_assert(false, "Should be unreachable");
// }

template <size_t szA, typename dtype, size_t szB, typename stype, bool, bool>
void gx_blit_loop( const gxDrawCmd &cmd )
{
    // static_assert(std::is_same_v<dst_type, u8vec4>, "rip");
    // static_assert(std::is_same_v<src_type, u8vec4> | std::is_same_v<src_type, u8vec3>, "rip");
    auto &bcmd = cmd.data.blit;
    // vec4 tint = cmd.conf.tint;
    // float *depth = (float*)(gxGetTexture(cmd.conf.depth)->data);

    auto *dtex = gxGetTexture(bcmd.dst);
    auto *dst  = (vec<szA, dtype>*)(dtex->data);
    int   dstw = dtex->w;
    auto &dstrect = bcmd.dstrect;

    auto *stex = gxGetTexture(bcmd.src);
    auto *src  = (vec<szB, stype>*)(stex->data);
    int   srcw = stex->w;
    auto &srcrect = bcmd.srcrect;

    // int xmin = std::max(dstrect.x, 0);
    // int xmax = std::min(dstrect.x+dstrect.w, dtex->w);
    // int ymin = std::max(dstrect.y, 0);
    // int ymax = std::min(dstrect.y+dstrect.h, dtex->h);

    // int xmin = std::max(dstrect.x, 0);
    // int xmax = std::min(dstrect.x+dstrect.w, dtex->w-1);
    // int ymin = std::max(dstrect.y, 0);
    // int ymax = std::min(dstrect.y+dstrect.h, dtex->h-1);

    // int srcx=0, srcy=0;
    // int dstx=0, dsty=0;
    // int dstidx=0, srcidx=0;
    // float alpha = 0.0f;

    // int jmax = ymax-ymin;
    // int imax = xmax-xmin;
    // int srcymin = srcrect.y;
    // int srcymax = srcrect.y+srcrect.h;
    // int srcxmin = srcrect.x;
    // int srcxmax = srcrect.x+srcrect.w;
    constexpr bool int_flt = std::is_integral_v<dtype> && std::is_floating_point_v<stype>;
    constexpr bool flt_int = std::is_floating_point_v<dtype> && std::is_integral_v<stype>;

    vec<szA, dtype> srcColor;

    // for (int dsty=ymin; dsty<ymax; dsty++)
    for (int i=0; i<dstrect.h; i++)
    {
        int dst_y = std::clamp(dstrect.y+i, 0, dtex->h-1);
        int src_y = std::clamp(srcrect.y+i, 0, stex->h-1);

        // for (int dstx=xmin; dstx<xmax; dstx++)
        for (int j=0; j<dstrect.w; j++)
        {
            int dst_x = std::clamp(dstrect.x+j, 0, dtex->w-1);
            int src_x = std::clamp(srcrect.x+j, 0, stex->w-1);
    
            int dstidx = dstw*dst_y + dst_x;
            int srcidx = srcw*src_y + src_x;

            // if constexpr (depth_test)
            // {
            //     if (depth[dstidx] < dstrect.z)
            //         continue;
            //     depth[dstidx] = dstrect.z;
            // }

            // // if constexpr (std::is_integral_v<dtype> == std::is_integral_v<stype>)
            if      constexpr (int_flt)  srcColor = vec<szA, dtype>(255.0f * src[srcidx]);
            else if constexpr (flt_int)  srcColor = vec<szA, dtype>(src[srcidx]);
            else                         srcColor = src[srcidx];

            // srcColor = vec<szA, dtype>(src[srcidx]);

            // if constexpr (alpha_blend && szA==4 && szB==4)
            //     dst[dstidx] = gx_mix(dst[dstidx], srcColor);
            // else
                dst[dstidx] = srcColor;
        }
    }
}




template <size_t szA, typename dtype, size_t szB, typename stype>
static void blit_xx_yy( const gxDrawCmd &cmd )
{
    using fn_type = void (*)( const gxDrawCmd& );
    static fn_type table[4] = {
        gx_blit_loop<szA, dtype, szB, stype, false, false>,
        gx_blit_loop<szA, dtype, szB, stype, false, true>,
        gx_blit_loop<szA, dtype, szB, stype, true,  false>,
        gx_blit_loop<szA, dtype, szB, stype, true,  true>
    };

    int idx = 0;
    if (cmd.conf.blend) idx |= 1;
    if (cmd.conf.depth) idx |= 2;

    table[idx](cmd);
}


// static void blit_u8vec3_xx( const gxDrawCmd &cmd )
// {
//     auto &bcmd = cmd.data.blit;
//     auto *src = gxGetTexture(bcmd.src);

//     switch (src->format)
//     {
//         default: kpanic("[blit_u8vec4_xx] invalid src->format"); break;
//         // case GX_R8:      blit_xx_yy<u8vec3, uint8_t>(cmd); break;
//         // case GX_RGB8:    blit_xx_yy<u8vec3, u8vec3>(cmd);  break;
//         case GX_RGBA8:   blit_xx_yy<u8vec3, u8vec4>(cmd);  break;
//         // case GX_RGBA32F: blit_xx_yy<u8vec3, vec4>(cmd);    break;
//     }
// }

template <size_t N, typename dtype>
static void blit_xx( const gxDrawCmd &cmd )
{
    auto &bcmd = cmd.data.blit;
    auto *src = gxGetTexture(bcmd.src);

    switch (src->format)
    {
        default: syslog::kprintf("RIP: %u\n", src->format); kpanic("[blit_u8vec4_xx] invalid src->format"); break;
        // case GX_R8:      blit_xx_yy<u8vec4, uint8_t>(cmd); break;
        case GX_RGB8:    blit_xx_yy<N, dtype, 3, uint8_t>(cmd);  break;
        case GX_RGBA8:   blit_xx_yy<N, dtype, 4, uint8_t>(cmd);  break;
        // case GX_RGBA32F: blit_xx_yy<u8vec4, vec4>(cmd);    break;
    }
}


// static void blit_vec4_xx( const gxDrawCmd &cmd )
// {
//     auto &bcmd = cmd.data.blit;
//     auto *src  = gxGetTexture(bcmd.src);

//     switch (src->format)
//     {
//         default: kpanic("[blit_vec4_xx] invalid src->format"); break;
//         // case GX_R8:      blit_xx_yy<vec4, uint8_t>(cmd); break;
//         // case GX_RGB8:    blit_xx_yy<vec4, u8vec3>(cmd);  break;
//         case GX_RGBA8:   blit_xx_yy<vec4, u8vec4>(cmd);  break;
//         case GX_RGBA32F: blit_xx_yy<vec4, vec4>(cmd);    break;
//     }
// }


void gx_ExecCommand_Blit( const gxDrawCmd &cmd )
{
    auto &bcmd = cmd.data.blit;
    auto *dst = gxGetTexture(bcmd.dst);
    // auto *src = gxGetTexture(bcmd.src);
    // syslog log("gx_ExecCommand_Blit: dst=%u, src=%u", dst->format, src->format);

    switch (dst->format)
    {
        default: kpanic("[gx_ExecCommand_Blit] Invalid dst format"); break;
        case GX_RGB8:    blit_xx<3, uint8_t>(cmd); break;
        case GX_RGBA8:   blit_xx<4, uint8_t>(cmd); break;
        // case GX_RGBA32F: blit_xx<1, float>(cmd);   break;
    }

}

