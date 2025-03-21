#include "pic.hpp"
#include "serial.hpp"

// https://wiki.osdev.org/Inline_Assembly/Examples#I/O_access

#define IRQ_BASE    0x20

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

void
idk::PIC::remap( int offset1, int offset2 )
{
	IO::outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	IO::wait();

	IO::outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
	IO::wait();

	IO::outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	IO::wait();

	IO::outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	IO::wait();

	IO::outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	IO::wait();

	IO::outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	IO::wait();
	
	IO::outb(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	IO::wait();

	IO::outb(PIC2_DATA, ICW4_8086);
	IO::wait();

	// Unmask both PICs.
	IO::outb(PIC1_DATA, 0);
	IO::outb(PIC2_DATA, 0);
}



void idk::PIC::setmask(uint8_t IRQline)
{
	uint16_t port;
	uint8_t value;

	port    = (IRQline < 8) ? PIC1_DATA : PIC2_DATA;
	IRQline = (IRQline < 8) ? IRQline   : IRQline-8;

	value = IO::inb(port) | (1 << IRQline);
	IO::outb(port, value);        
}



void idk::PIC::unmask( uint8_t IRQline )
{
	uint16_t port;
	uint8_t value;

	port    = (IRQline < 8) ? PIC1_DATA : PIC2_DATA;
	IRQline = (IRQline < 8) ? IRQline   : IRQline-8;

	value = IO::inb(port) & ~(1 << IRQline);
	IO::outb(port, value);        
}

void idk::PIC::disable()
{
    IO::outb(PIC1_DATA, 0xFF);
    IO::outb(PIC2_DATA, 0xFF);
}

void idk::PIC::sendEOI( uint8_t irq )
{
	if (irq >= 8)
	{
		IO::outb(PIC2_CMD, 0x20);
	}
		
	IO::outb(PIC1_CMD, 0x20);
}

