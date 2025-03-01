#pragma once

#include <stddef.h>
#include <stdint.h>


namespace ck::serial
{
    static constexpr uint16_t COM1 = 0x3F8;
    static constexpr uint16_t COM2 = 0x2F8;
    static constexpr uint16_t COM3 = 0x3E8;
    static constexpr uint16_t COM4 = 0x2E8;
    static constexpr uint16_t COM5 = 0x5F8;
    static constexpr uint16_t COM6 = 0x4F8;
    static constexpr uint16_t COM7 = 0x5E8;
    static constexpr uint16_t COM8 = 0x4E8;

    int init();
    int writestr( const char* );

    uint8_t inb( uint16_t port );
    uint16_t inw( uint16_t port );

    void outb(uint16_t port, uint8_t val);
    void outw(uint16_t port, uint16_t val);
}
