#include <driver/serial.hpp>
#include <arch/io.hpp>
#include <kernel/log.hpp>
#include <stdio.h>
#include <string.h>


bool serial::init()
{
    using namespace IO;

    IO::out8(COM1+1, 0x00);    // Disable all interrupts
    IO::out8(COM1+3, 0x80);    // Enable DLAB (set baud rate divisor)
    IO::out8(COM1+0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    IO::out8(COM1+1, 0x00);    //                  (hi byte)
    IO::out8(COM1+3, 0x03);    // 8 bits, no parity, one stop bit
    IO::out8(COM1+2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    // IO::out8(COM1+4, 0x0B);    // IRQs enabled, RTS/DSR set
    IO::out8(COM1+4, 0x1E);    // Set in loopback mode, test the serial chip
    IO::out8(COM1+0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
    // // Check if serial is faulty (i.e: not same byte as sent)
    if (IO::in8(COM1 + 0) != 0xAE)
    {
        // syslog::disable();
        return false;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    IO::out8(COM1 + 4, 0x0F);

    return true;
}


