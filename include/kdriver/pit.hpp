#pragma once

#include <stddef.h>
#include <stdint.h>


namespace idk
{
    static constexpr uint16_t PIT_HERTZ     = uint16_t(1000);
    static constexpr uint16_t PIT_FREQUENCY = uint16_t(1193182);

    void     PIT_init();
    uint32_t PIT_read();
    bool     PIT_edge();

}

