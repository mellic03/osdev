#pragma once
#include <kmath/vec.hpp>

struct gx_RenderTarget
{
    uint32_t color;
    uint32_t depth;
    uint32_t stencil;
};

struct gx_Context
{
    gx_RenderTarget target;

    struct Conf {
        bool blend   = false;
        bool depth   = false;
        bool stencil = false;
        vec4 tint    = vec4(1.0f);
    } conf;

    ivec4 viewport     = ivec4(0);
    vec4  clearcolor   = vec4(0.0f);
    float cleardepth   = 1.0f;
    int   clearstencil = 0;
};


void        gxMakeCurrent( gx_Context* );
gx_Context *gxGetCurrent();
void        gxPushContext();
void        gxPopContext();

