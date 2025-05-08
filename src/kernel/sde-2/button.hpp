#pragma once
#include "frame.hpp"

namespace sde
{
    class Button;
    // sde::Frame *makeButton( int x, int y, int w, int h );
}


class sde::Button: public sde::Frame
{
private:

public:
    vec4 m_fillColor;
    vec4 m_fillColorHovered;

    Button( int x, int y, int w, int h );

    virtual void update() final;
    virtual void draw() final;
};
