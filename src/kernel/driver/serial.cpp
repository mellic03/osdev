#include <kdriver/serial.hpp>

#include <stdio.h>
#include <string.h>

static char __serial_buf[256];
static int  __syslog_indent;


kret_t
idk::serial_init()
{
    using namespace IO;

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
        return KRET_FAILURE;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    IO::outb(COM1 + 4, 0x0F);

    return KRET_SUCCESS;
}


int __pushIndent( int n )
{
    int prev = __syslog_indent;
    __syslog_indent += n;
    return prev;
}




uint8_t idk::IO::inb( uint16_t port )
{
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint16_t idk::IO::inw( uint16_t port )
{
    uint16_t ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint32_t idk::IO::inl( uint16_t port )
{
    uint32_t ret;
    __asm__ __volatile__ ("inl %w1,%0":"=a" (ret):"Nd" (port));
    return ret;
}


void idk::IO::outb( uint16_t port, uint8_t value )
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}


void idk::IO::outw( uint16_t port, uint16_t value )
{
    __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}


void idk::IO::outl( uint16_t port, uint32_t value )
{
    __asm__ volatile("outl %0, %w1" : : "a"(value), "Nd"(port));
}


void idk::IO::wait()
{
    IO::outb(0x80, 0x00);
}






void idk::serialf( const char *fmt, ... )
{
    va_list args;
    va_start (args, fmt);
    int n = vsprintf(__serial_buf, fmt, args);
    __serial_buf[n-1] = '\n';
    __serial_buf[n]   = '\0';
    va_end(args);

    for (int i=0; i<__syslog_indent; i++)
    {
        idk::IO::outb(idk::IO::COM1, ' ');
    }

    for (int i=0; i<n; i++)
    {
        idk::IO::outb(idk::IO::COM1, __serial_buf[i]);
    }
}








void __popIndent( int n )
{
    __syslog_indent -= n;
    __syslog_indent = (__syslog_indent < 0) ? 0 : __syslog_indent;
}

extern "C"
{
    void SYSLOG_BEGIN( const char *title )
    {
        SYSLOG("[%s]", title);
        SYSLOG("{");
        __pushIndent(4);
    }


    void SYSLOG( const char *fmt, ... )
    {
        va_list args;
        va_start (args, fmt);
        int n = vsprintf(__serial_buf, fmt, args);
        __serial_buf[n-1] = '\n';
        __serial_buf[n]   = '\0';
        va_end(args);

        for (int i=0; i<__syslog_indent; i++)
        {
            idk::IO::outb(idk::IO::COM1, ' ');
        }

        for (int i=0; i<n; i++)
        {
            idk::IO::outb(idk::IO::COM1, __serial_buf[i]);
        }
    }


    void SYSLOG_END()
    {
        __popIndent(4);
        SYSLOG("}");
    }


}
