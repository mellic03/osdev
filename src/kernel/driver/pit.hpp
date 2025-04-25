#pragma once

#include <stddef.h>
#include <stdint.h>
#include "interface.hpp"


struct hwdi_PIT: public hwDriverInterface
{
    inline static uint16_t timer_ms = 5;

    hwdi_PIT( uint16_t ms );
    virtual void loadIrqHandler() final;
};



static constexpr uint32_t PIT_FREQUENCY = uint32_t(1193182);
extern uint16_t PIT_HERTZ;

namespace PIT
{
    void init();
    void reload();

    void set_hz( uint16_t hz );
    void set_ms( uint16_t ms );

    uint16_t read();
    bool     edge();

}

