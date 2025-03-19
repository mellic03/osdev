#include "./serial.hpp"
#include "../io.hpp"

#include <stdio.h>
#include <string.h>

static char __serial_buf[256];
static int  __syslog_indent;


void
idk::serial::init()
{
    memset(__serial_buf, '\0', 256);
    __syslog_indent = 0;

    IO::outb(COM1+1, 0x00);    // Disable all interrupts
    IO::outb(COM1+3, 0x80);    // Enable DLAB (set baud rate divisor)
    IO::outb(COM1+0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    IO::outb(COM1+1, 0x00);    //                  (hi byte)
    IO::outb(COM1+3, 0x03);    // 8 bits, no parity, one stop bit
    IO::outb(COM1+2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    IO::outb(COM1+4, 0x0B);    // IRQs enabled, RTS/DSR set
    IO::outb(COM1+4, 0x1E);    // Set in loopback mode, test the serial chip
    IO::outb(COM1+0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
    // // Check if serial is faulty (i.e: not same byte as sent)
    if (IO::inb(COM1 + 0) != 0xAE)
    {
        // return 0;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    IO::outb(COM1 + 4, 0x0F);
}


int
idk::serial::writeln( const char *str )
{
    while (*str)
    {
        IO::outb(COM1, *str);
        str++;
    }

    IO::outb(COM1, '\0');

    return 0;
}





int serial_printf( const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    int n = vsprintf(__serial_buf, fmt, args);
    va_end(args);

    for (int i=0; i<__syslog_indent; i++)
    {
        idk::IO::outb(idk::serial::COM1, ' ');
    }

    for (int i=0; i<n; i++)
    {
        idk::IO::outb(idk::serial::COM1, __serial_buf[i]);
    }
    
    return n;
}


int syslog_pushIndent( int n )
{
    int prev = __syslog_indent;
    __syslog_indent += n;
    return prev;
}


void syslog_popIndent( int n )
{
    __syslog_indent -= n;
    __syslog_indent = (__syslog_indent < 0) ? 0 : __syslog_indent;
}


void syslog_clearIndent()
{
    __syslog_indent = 0;
}
