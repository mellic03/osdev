#pragma once

#include <stddef.h>
#include <stdint.h>
// #include "interface.hpp"


// struct hwdi_PIT: public hwDriverInterface
// {
//     inline static uint16_t timer_ms = 5;

//     hwdi_PIT( uint16_t ms );
//     virtual void loadIrqHandler() final;
// };


namespace PIT
{
    static constexpr uint32_t FREQUENCY = uint32_t(1193182);
    extern uint32_t CurrFrequency;
    extern uint16_t HERTZ;

    void init( uint32_t new_frequency  );
    // void set_hz( uint16_t hz );
    // void set_ms( uint16_t ms );
    uint32_t sleep( uint64_t ms );

    // uint16_t read();
    // bool     edge();

}

