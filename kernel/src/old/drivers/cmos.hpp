#pragma once

#include <stddef.h>
#include <stdint.h>


namespace idk
{
    struct CMOS_time
    {
        uint8_t seconds, minutes, hours;
        uint8_t weekday, monthday, month;
        uint8_t year;
        uint8_t century;
    };

    CMOS_time CMOS_read();

}

