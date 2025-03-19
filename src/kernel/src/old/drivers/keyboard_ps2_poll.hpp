#pragma once

#include <stddef.h>
#include <stdint.h>


namespace idk
{
    class keyboard_ps2_poll;
}

class idk::keyboard_ps2_poll
{
private:

public:
    uint8_t state[256];

    keyboard_ps2_poll();
    uint8_t poll();

};

