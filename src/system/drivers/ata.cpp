#include "ata.hpp"
#include "../io.hpp"
#include "./serial.h"


void idk::ata::test()
{
    IO::outb(0x1F6, 0xB0);

    IO::outb(0x1F2, 0);
    IO::outb(0x1F3, 0);
    IO::outb(0x1F4, 0);
    IO::outb(0x1F5, 0);

    IO::outb(0x1F7, 0xEC);

    uint8_t res = IO::inb(0x1F7);


    if (res == 0)
    {
        serial_printf("[ata::test] res==0, non-ATA\n");
        return;
    }

    serial_printf("[ata::test] res!=0, detected ATA\n");


    // poll the Status port (0x1F7) until bit 7 (BSY, value = 0x80) clears
    while (true)
    {
        res = IO::inb(0x1F7);
    
        if ((res & 0x80) == 0)
        {
            break;
        }
    }

    uint8_t status = 0;
    uint8_t err    = 0;

    while (true)
    {
        res = IO::inb(0x1F7);
    
        status = (res & 0x08);
        err    = (res & 0x01);

        if (status || err)
        {
            break;
        }
    }

    if (err)
    {
        serial_printf("[ata::test] err==1, read error\n");
        return;
    }

    serial_printf("[ata::test] status==8, read success\n");


    static uint16_t buf[256];

    serial_printf("\n");
    for (int i=0; i<256; i++)
    {
        buf[i] = IO::inw(0x1F0);
        serial_printf("0x%x\n", buf[i]);
    }

}
