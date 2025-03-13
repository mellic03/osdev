#pragma once

#include <stddef.h>
#include <stdint.h>


namespace ck::PS2
{
    static constexpr uint8_t PORT_DATA    = 0x60; 
    static constexpr uint8_t PORT_STATUS  = 0x64; 
    static constexpr uint8_t PORT_COMMAND = 0x64;

    void send( uint8_t cmd, uint8_t data );
    uint8_t read();
    uint8_t status();

}
