#include <kernel/ioport.hpp>
#include <string.h>


uint8_t IO::inb( uint16_t port )
{
    // return serial_inb(port);
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint16_t IO::inw( uint16_t port )
{
    uint16_t ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint32_t IO::inl( uint16_t port )
{
    uint32_t ret;
    __asm__ volatile ("inl %w1,%0":"=a" (ret):"Nd" (port));
    return ret;
}


void IO::outb( uint16_t port, uint8_t value )
{
    // serial_outb(port, value);
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}


void IO::outw( uint16_t port, uint16_t value )
{
    __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}


void IO::outl( uint16_t port, uint32_t value )
{
    __asm__ volatile("outl %0, %w1" : : "a"(value), "Nd"(port));
}


void IO::wait()
{
    IO::outb(0x80, 0x00);
}





// void idk::__serialf( const char *fmt, va_list args )
// {
//     int n = vsprintf(__serial_buf, fmt, args);

//     for (int i=0; i<__syslog_indent; i++)
//     {
//         IO::outb IO::COM1, ' ');
//     }

//     for (int i=0; i<n; i++)
//     {
//         IO::outb IO::COM1, __serial_buf[i]);
//     }
// }




// void idk::serialf( const char *fmt, ... )
// {
//     va_list args;
//     va_start (args, fmt);
//     __serialf(fmt, args);
//     va_end(args);
// }








// void __popIndent( int n )
// {
//     __syslog_indent -= n;
//     __syslog_indent = (__syslog_indent < 0) ? 0 : __syslog_indent;
// }

// void SYSLOG_BEGIN( const char *title )
// {
//     SYSLOG("[%s]", title);
//     SYSLOG("{");
//     __pushIndent(4);
// }


// void SYSLOG( const char *fmt, ... )
// {
//     va_list args;
//     va_start (args, fmt);
//     idk::__serialf(fmt, args);
//     idk::serialf("\n");
//     va_end(args);
// }


// void SYSLOG_END()
// {
//     __popIndent(4);
//     SYSLOG("}");
// }


