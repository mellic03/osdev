#pragma once

#include <stddef.h>
#include <stdint.h>


namespace idk
{
    class ps2_keyboard;
}

class idk::ps2_keyboard
{
private:

public:
    uint8_t state[256];

    ps2_keyboard();
    uint8_t poll();

};

