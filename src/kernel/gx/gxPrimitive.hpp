#pragma once
#include <kmath/vec.hpp>

struct gxTrianglePrimitive
{
    vec3  pos[3];
    float invz[3]; // inverse z of vertex 0, 1, 2
    float invw[3]; // inverse w of vertex 0, 1, 2
    vec3  norm[3];
    vec2  uv[3];
};
