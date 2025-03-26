#pragma once

#include <stddef.h>
#include <stdint.h>


namespace idk
{
    static constexpr uint32_t PIT_FREQUENCY = uint32_t(1193182);
    extern uint16_t PIT_HERTZ;

    void     PIT_init();
    void     PIT_reload();

    void     PIT_set_hz( uint16_t hz );
    void     PIT_set_ms( uint16_t ms );

    uint16_t PIT_read();
    bool     PIT_edge();

}

