// #include <kernel/ioport.hpp>
// #include <string.h>


// uint8_t IO::inb( uint16_t port )
// {
//     uint8_t ret;
//     __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
//     return ret;
// }

// uint16_t IO::inw( uint16_t port )
// {
//     uint16_t ret;
//     __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
//     return ret;
// }

// uint32_t IO::inl( uint16_t port )
// {
//     uint32_t ret;
//     __asm__ volatile ("inl %w1,%0":"=a" (ret):"Nd" (port));
//     return ret;
// }


// void IO::outb( uint16_t port, uint8_t value )
// {
//     __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
// }


// void IO::outw( uint16_t port, uint16_t value )
// {
//     __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
// }


// void IO::outl( uint16_t port, uint32_t value )
// {
//     __asm__ volatile("outl %0, %w1" : : "a"(value), "Nd"(port));
// }






// uint8_t IO::in8( uint16_t port )
// {
//     uint8_t ret;
//     __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
//     return ret;
// }

// uint16_t IO::in16( uint16_t port )
// {
//     uint16_t ret;
//     __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
//     return ret;
// }

// uint32_t IO::in32( uint16_t port )
// {
//     uint32_t ret;
//     __asm__ volatile ("inl %w1,%0":"=a" (ret):"Nd" (port));
//     return ret;
// }

// void IO::out8( uint16_t port, uint8_t value )
// {
//     __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
// }


// void IO::out16( uint16_t port, uint16_t value )
// {
//     __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
// }


// void IO::out32( uint16_t port, uint32_t value )
// {
//     __asm__ volatile("outl %0, %w1" : : "a"(value), "Nd"(port));
// }


// void IO::wait()
// {
//     IO::outb(0x80, 0);
// }

