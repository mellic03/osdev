#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kernel/ioport.hpp>

namespace PIC
{
    static constexpr uint8_t IRQ_MASTER = 32;
    static constexpr uint8_t IRQ_SLAVE  = 40;

    static constexpr uint16_t PIC1_CMD  = 0x0020;
    static constexpr uint16_t PIC1_DATA = 0x0021;
    static constexpr uint16_t PIC2_CMD  = 0x00A0;
    static constexpr uint16_t PIC2_DATA = 0x00A1;
    
    static constexpr uint8_t ICW1_ICW4       = 0x01; // ICW4 command word: 0 = not needed, 1 = needed
    static constexpr uint8_t ICW1_SINGLE     = 0x02; // Single mode: 0 = cascade, 1 = single
    static constexpr uint8_t ICW1_ADI        = 0x04; // Call address interval: 0 = interval of 8, 1 = interval of 4
    static constexpr uint8_t ICW1_LTIM       = 0x08; // Interrupt trigger mode: 0 = edge, 1 = level
    static constexpr uint8_t ICW1_INIT       = 0x10; // Initialization
    
    static constexpr uint8_t ICW4_8086       = 0x01; // Microprocessor mode: 0=MCS-80/85, 1=8086/8088
    static constexpr uint8_t ICW4_AUTO       = 0x02; // Auto EOI: 0 = disabled, 1 = enabled
    static constexpr uint8_t ICW4_BUF_SLAVE  = 0x04; // Buffered mode/slave
    static constexpr uint8_t ICW4_BUF_MASTER = 0x0C; // Buffered mode/master
    static constexpr uint8_t ICW4_SFNM       = 0x10; // Special fully nested is programmed
    
    void remap( int master_offset, int slave_offset );
    void setmask(uint8_t IRQline);
    void unmask(uint8_t IRQline);
    void disable();
    void sendEOI( uint8_t irq );


}
