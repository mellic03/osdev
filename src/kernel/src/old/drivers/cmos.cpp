#include "cmos.hpp"
#include "io.hpp"
#include "log.hpp"



uint8_t inoutb( uint16_t outport, uint16_t inport, uint8_t outval )
{
    idk::IO::outb(outport, outval);
    return idk::IO::inb(inport);
}


idk::CMOS_time
idk::CMOS_read()
{
    SYSLOG_BEGIN("idk::CMOS_read");

    CMOS_time time;

    time.seconds  = inoutb(0x70, 0x71, 0x00);
    time.minutes  = inoutb(0x70, 0x71, 0x02);
    time.hours    = inoutb(0x70, 0x71, 0x04);

    time.weekday  = inoutb(0x70, 0x71, 0x06);
    time.monthday = inoutb(0x70, 0x71, 0x07);
    time.month    = inoutb(0x70, 0x71, 0x08);
    time.year     = inoutb(0x70, 0x71, 0x09);

    if (time.seconds >= 60)
    {
        time.seconds -= 60;
        time.minutes += 1;
    }

    if (time.minutes >= 60)
    {
        time.minutes -= 60;
        time.hours   += 1;
    }

    if (time.hours >= 24)
    {
        time.hours -= 24;
    }

    // uint8_t statusB = inoutb(0x70, 0x71, 0x0B);

    // {
    //     time.hours = ((time.hours & 0x7F) + 12) % 24;
    // }

    SYSLOG(
        "time: %uy %um %ud  %uh %um %us\n",
        time.year, time.month, time.monthday,
        time.hours, time.minutes, time.seconds
    );

    SYSLOG_END

    return time;
}