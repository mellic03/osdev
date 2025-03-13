#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "./serial.h"
#include "../memory/base_allocator.hpp"



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

    int init( idk::base_allocator* );
    int writeln( const char* );

}