#pragma once

#include "../kvideo/kvideo.hpp"
#include "frame.hpp"
#include <kmath/vec.hpp>
#include <vector>

namespace sde
{
    class Frame;
    class WindowContext;
}


class sde::WindowContext: public sde::Frame
{
private:
    inline static int ctx_id = 0;
public:
    const int ID;
    const int W;
    const int H;

    kframebuffer<vec4>     rgba;
    kframebuffer<uint32_t> depth;

    WindowContext( ivec2 tl, ivec2 sp );
    ~WindowContext();

    virtual void draw() final;
    void clear();
};
