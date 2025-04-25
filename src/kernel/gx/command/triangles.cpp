#include <kernel/log.hpp>
#include <kpanic.h>

#include "../gx.hpp"
#include "../gx_assign.hpp"
#include "../gx_mix.hpp"
#include "../../kvideo/kvideo.hpp"

void gx_rasterize( gxTexture *dst, const gxTrianglePrimitive& );


void gx_ExecCommand_Tris( const gxDrawCmd &cmd )
{
    auto &tcmd = cmd.data.tris;
    auto *tex  = gxGetTexture(tcmd.dst);
    auto *buf  = (gxTrianglePrimitive*)gxGetBuffer(tcmd.src);

    for (size_t i=0; i<tcmd.count; i++)
    {
        size_t idx = tcmd.offset + i;
        gx_rasterize(tex, buf[idx]);
    }
}









template <typename T>
T min3( T a, T b, T c )
{
    return std::min(a, std::min(b, c));
}

template <typename T>
T max3( T a, T b, T c )
{
    return std::max(a, std::max(b, c));
}



struct BarData
{
    float v1x, v1y, v2x, v2y, v3x, v3y;
    float denom;
};

struct baryp_coeff
{
    float v1x, v1y, v2x, v2y, v3x, v3y;
    float a, b, c, d;
    float denom;
};

static baryp_coeff
BarData_load( const gxTrianglePrimitive &p )
{
    baryp_coeff B;

    B.v1x = p.pos[0].x, B.v1y = p.pos[0].y;
    B.v2x = p.pos[1].x, B.v2y = p.pos[1].y;
    B.v3x = p.pos[2].x, B.v3y = p.pos[2].y;

    B.denom = (B.v2y-B.v3y)*(B.v1x-B.v3x) + (B.v3x-B.v2x)*(B.v1y-B.v3y);

    return B;
}

static void
barycentric2D( float x, float y, const baryp_coeff &B, float *weights )
{
    weights[0] = ((B.v2y-B.v3y)*(x-B.v3x) + (B.v3x-B.v2x)*(y-B.v3y)) / B.denom;
    weights[1] = ((B.v3y-B.v1y)*(x-B.v3x) + (B.v1x-B.v3x)*(y-B.v3y)) / B.denom;
    weights[2] = 1.0f - weights[0] - weights[1];
}

template <typename T>
T baryp_ptr( const T *data, float *weights )
{
    return data[0]*weights[0] + data[1]*weights[1] + data[2]*weights[2];
}





// static uint8_t
// vert_process( const gxTrianglePrimitive &p0, gxTrianglePrimitive &p1, int idx )
// {
//     using namespace glm;

//     const mat4 &P = un.P;
//     const mat4 &V = un.V;
//     const mat4 &T = un.T;
//     const vec2 &viewport = un.viewport;

//     // vec3 view = VT * vec4(p0.pos[idx], 1.0f);

//     // if (view.z > 0.01f)
//     // {
//     //     return 0;
//     // }

//     vec4  proj  = P * V * T * vec4(p0.pos[idx], 1.0f);
//     float inv_w = 1.0f  / std::max(proj.w, 0.01f);
//     vec3  ndc   = vec3(proj) * inv_w;
//     vec2  scr   = viewport * (vec2(ndc) * 0.5f + 0.5f);
//           scr.y = viewport.y - scr.y;

//     p1.pos[idx]  = vec3(scr, 1.0f / std::max(ndc.z, 0.01f));
//     p1.invz[idx] = 1.0f / std::max(ndc.z, 0.01f);
//     p1.invw[idx] = inv_w;

//     p1.norm[idx] = mat3(T) * p0.norm[idx];
//     p1.uv[idx]   = p0.uv[idx] * inv_w;

//     return 1;
// }

// static bool
// prim_process( const sven::Primitive &p0, sven::Primitive &p1, const sven::CTX_uniforms &un )
// {
//     uint8_t result = 1;

//     for (int i=0; i<3; i++)
//     {
//         result *= vert_process(p0, p1, i, un);
//     }

//     return (result == 1);
// }






void gx_rasterize( gxTexture *dst, const gxTrianglePrimitive &P )
{
    auto TILE_W = dst->w;
    auto TILE_H = dst->h;

    int xmin = int(min3(P.pos[0].x, P.pos[1].x, P.pos[2].x));
    int xmax = int(max3(P.pos[0].x, P.pos[1].x, P.pos[2].x));
    int ymin = int(min3(P.pos[0].y, P.pos[1].y, P.pos[2].y));
    int ymax = int(max3(P.pos[0].y, P.pos[1].y, P.pos[2].y));

    if (xmax < 0 || xmin > TILE_W-1) { return; }
    if (ymax < 0 || ymin > TILE_H-1) { return; }

    xmin = std::clamp(xmin, 0, TILE_W-1);
    xmax = std::clamp(xmax, 0, TILE_W-1);
    ymin = std::clamp(ymin, 0, TILE_W-1);
    ymax = std::clamp(ymax, 0, TILE_W-1);

    baryp_coeff coeff = BarData_load(P);
    float       weights[3];

    vec4 gx_FragCoord;
    // vec2 gx_TexCoord;

    for (int y=ymin; y<=ymax; y++)
    {
        for (int x=xmin; x<=xmax; x++)
        {
            barycentric2D(x, y, coeff, weights);

            if (weights[0] < 0.0f || weights[1] < 0.0f || weights[2] < 0.0f)
            {
                continue;
            }


            gx_FragCoord.x = float(x); // / float(dstw);
            gx_FragCoord.y = float(y); // / float(dsth);
            // gx_FragCoord.z = 1.0f / baryp_ptr(P.invz, weights);
            // gx_FragCoord.w = 1.0f / baryp_ptr(P.invw, weights);

            // if (1.0f / gx_FragCoord.z <= dst_depth[TILE_W*y + x])
            // {
            //     continue;
            // }

            // dst_depth[TILE_W*y + x] = 1.0f / gx_FragCoord.z;

            // vec4 fragcoord = gx_FragCoord;
            // vec3 normal    = gx_FragCoord.w * baryp_ptr(P.norm, weights);
            // vec2 texcoord  = gx_FragCoord.w * baryp_ptr(P.uv, weights);
            // gx_TexCoord = baryp_ptr(P.uv, weights);
            vec2 gx_TexCoord = baryp_ptr(P.uv, weights);
            vec4 color     = vec4(gx_TexCoord, gx_TexCoord.x * gx_TexCoord.y, 1.0f);

            // dst->data
            kvideo::backbuffer[TILE_W*y + x] = vec4_pack_argb(color);
        }
    }
}

/*
#define GX_FRAGMENT_TARGET(DstType)\
    auto &P = cmd.P;\
    auto *dtex = gxGetTexture(cmd.dst);\
    auto *dst  = (DstType*)(dtex->data);\
    int   dstw = dtex->w;\
    int   dsth = dtex->h;\

#define GX_FRAGMENT_SHADER(Content)\
{\
    auto TILE_W = dstw;\
    auto TILE_H = dsth;\
\
    int xmin = int(min3(P.pos[0].x, P.pos[1].x, P.pos[2].x));\
    int xmax = int(max3(P.pos[0].x, P.pos[1].x, P.pos[2].x));\
    int ymin = int(min3(P.pos[0].y, P.pos[1].y, P.pos[2].y));\
    int ymax = int(max3(P.pos[0].y, P.pos[1].y, P.pos[2].y));\
\
    if (xmax < 0 || xmin > TILE_W-1) { return; }\
    if (ymax < 0 || ymin > TILE_H-1) { return; }\
\
    xmin = std::clamp(xmin, 0, TILE_W-1);\
    xmax = std::clamp(xmax, 0, TILE_W-1);\
    ymin = std::clamp(ymin, 0, TILE_W-1);\
    ymax = std::clamp(ymax, 0, TILE_W-1);\
\
    baryp_coeff coeff = BarData_load(P);\
    float       weights[3];\
\
    vec4 gx_FragCoord;\
    vec2 gx_TexCoord;\
\
    for (int y=ymin; y<=ymax; y++)\
    {\
        for (int x=xmin; x<=xmax; x++)\
        {\
            barycentric2D(x, y, coeff, weights);\
            \
            if (weights[0] < 0.0f || weights[1] < 0.0f || weights[2] < 0.0f)\
            {\
                continue;\
            }\
            gx_FragCoord.x = float(x) / float(dstw);\
            gx_FragCoord.y = float(y) / float(dsth);\
            gx_TexCoord    = baryp_ptr(P.uv, weights);\
            auto &gx_FragColor = dst[TILE_W*y + x];\
            Content\
        }\
    }\
}\


// static void fragtest( const gxDrawCmdTriangle &cmd )
// {
//     GX_FRAGMENT_TARGET(vec4)

//     GX_FRAGMENT_SHADER(
//         vec2 uv = gx_TexCoord;
//         gx_FragColor = vec4(uv, uv.x*uv.y, 1.0f);
//     )
// }


*/
