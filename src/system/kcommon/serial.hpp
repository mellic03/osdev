#pragma once
#include <stddef.h>
#include <stdint.h>


namespace ck::serial
{
    static constexpr uint16_t COM1 = 0x3F8;

    int init();
    int writemsg( const char* );

    uint8_t inb( uint16_t port );
    void outb(uint16_t port, uint8_t val);
}
