#pragma once

#define u8vec4_mix(u8dst, u8src)\
u8dst[0] = ((255-u8src.a)*u8dst[0] + u8src.a*u8src[0]) / 255;\
u8dst[1] = ((255-u8src.a)*u8dst[1] + u8src.a*u8src[1]) / 255;\
u8dst[2] = ((255-u8src.a)*u8dst[2] + u8src.a*u8src[2]) / 255;\
u8dst[3] = ((255-u8src.a)*u8dst[3] + u8src.a*u8src[3]) / 255;\

#define vec4_mix(f32dst, f32src)\
f32dst = (1.0f-f32src.a)*f32dst + f32src.a*f32src



#define GX_BLIT_DTYPE(TypeA, TypeB)\
    auto *dtex = gxGetTexture(cmd.dst);\
    auto *dst  = (TypeA*)(dtex->data);\
    int   dstw = dtex->w;\
    auto &dstrect = cmd.dstrect;\
    auto *stex = gxGetTexture(cmd.src);\
    auto *src  = (TypeB*)(stex->data);\
    int   srcw = stex->w;\
    auto &srcrect = cmd.srcrect;\

#define GX_BLIT_LOOP(Content)\
{\
    int xmin = std::max(dstrect.x, 0);\
    int xmax = std::min(dstrect.x+dstrect.w, dtex->w);\
    int ymin = std::max(dstrect.y, 0);\
    int ymax = std::min(dstrect.y+dstrect.h, dtex->h);\
    int srcx=0, srcy=0;\
    int dstx=0, dsty=0;\
    int dstidx=0, srcidx=0;\
    float alpha = 0.0f;\
    \
    int jmax = ymax-ymin;\
    int imax = xmax-xmin;\
    int srcymin = srcrect.y;\
    int srcymax = srcrect.y+srcrect.h;\
    int srcxmin = srcrect.x;\
    int srcxmax = srcrect.x+srcrect.w;\
    \
    for (int j=0; j<jmax; j++)\
    {\
        alpha = float(j) / float(jmax);\
        srcy  = int((1.0f-alpha)*srcymin + alpha*srcymax + 0.5f);\
        dsty  = ymin + j;\
        \
        for (int i=0; i<imax; i++)\
        {\
            alpha = float(i) / float(imax);\
            srcx  = int((1.0f-alpha)*srcxmin + alpha*srcxmax + 0.5f);\
            dstx  = xmin + i;\
            \
            if (depth[dstidx] < dstrect.z)\
                continue;\
            depth[dstidx] = dstrect.z;\
            \
            dstidx = dstw*dsty + dstx;\
            srcidx = srcw*srcy + srcx;\
            auto &gx_FragColor = dst[dstidx];\
            Content;\
        }\
    }\
}\



#define GX_RECT_DTYPE(TypeA)\
auto *dtex = gxGetTexture(cmd.dst);\
auto *dst  = (TypeA*)(dtex->data);\
int   dstw = dtex->w;\
auto &rect = cmd.rect;\
int xmin = std::max(rect.x, 0);\
int xmax = std::min(rect.x+rect.w, dtex->w);\
int ymin = std::max(rect.y, 0);\
int ymax = std::min(rect.y+rect.h, dtex->h);\
int dstidx=0;\

#define GX_RECT_LOOP(Content)\
{\
    for (int dsty=ymin; dsty<ymax; dsty++)\
    {\
        for (int dstx=xmin; dstx<xmax; dstx++)\
        {\
            if (depth[dstidx] < rect.z)\
                continue;\
            depth[dstidx] = rect.z;\
            dstidx = dstw*dsty + dstx;\
            auto &gx_FragColor = dst[dstidx];\
            Content;\
        }\
    }\
}\

#define GX_RECT_BORDER(Content)\
{\
    for (int dsty=ymin; dsty<ymax; dsty++)\
    {\
        for (int x=xmin; x<xmin+cmd.border; x++)\
        {\
            if (depth[dstidx] < rect.z)\
                continue;\
            depth[dstidx] = rect.z;\
            dstidx = dstw*dsty + x;\
            auto &gx_FragColor = dst[dstidx];\
            Content;\
        }\
        for (int x=xmax-cmd.border; x<xmax; x++)\
        {\
            if (depth[dstidx] < rect.z)\
                continue;\
            depth[dstidx] = rect.z;\
            dstidx = dstw*dsty + x;\
            auto &gx_FragColor = dst[dstidx];\
            Content;\
        }\
    }\
    for (int dstx=xmin; dstx<xmax; dstx++)\
    {\
        for (int y=ymin; y<ymin+cmd.border; y++)\
        {\
            if (depth[dstidx] < rect.z)\
                continue;\
            depth[dstidx] = rect.z;\
            dstidx = dstw*y + dstx;\
            auto &gx_FragColor = dst[dstidx];\
            Content;\
        }\
        for (int y=ymax-cmd.border; y<ymax; y++)\
        {\
            if (depth[dstidx] < rect.z)\
                continue;\
            depth[dstidx] = rect.z;\
            dstidx = dstw*y + dstx;\
            auto &gx_FragColor = dst[dstidx];\
            Content;\
        }\
    }\
}\




#define GX_FILL_DTYPE(TypeA)\
auto *dst = (TypeA*)(dtex->data);\

#define GX_FILL_LOOP(Content)\
size_t count = dtex->w * dtex->h;\
for (size_t i=0; i<count; i++)\
{\
    auto &gx_FragColor = dst[i];\
    Content;\
}\



