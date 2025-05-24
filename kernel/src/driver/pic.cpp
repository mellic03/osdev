#include <driver/pic.hpp>
#include <arch/io.hpp>
#include <kassert.h>

// https://wiki.osdev.org/Inline_Assembly/Examples#I/O_access

static bool pic_enabled = true;

void PIC::enable()
{
	pic_enabled = true;
}


void PIC::disable()
{
	PIC::maskAll();
	pic_enabled = false;
}


void
PIC::remap( int offset1, int offset2 )
{
	kassert(pic_enabled);

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
}


void PIC::maskAll()
{
	kassert(pic_enabled);

	IO::outb(PIC1_DATA, 0xFF);
    IO::outb(PIC2_DATA, 0xFF);
}


void PIC::setmask( uint8_t IRQline )
{
	kassert(pic_enabled);

	uint16_t port;
	uint8_t value;

	port    = (IRQline < 8) ? PIC1_DATA : PIC2_DATA;
	IRQline = (IRQline < 8) ? IRQline   : IRQline-8;

	value = IO::inb(port) | (1 << IRQline);
	IO::outb(port, value);        
}


void PIC::unmask( uint8_t IRQline )
{
	kassert(pic_enabled);

	uint16_t port;
	uint8_t value;

	port    = (IRQline < 8) ? PIC1_DATA : PIC2_DATA;
	IRQline = (IRQline < 8) ? IRQline   : IRQline-8;

	value = IO::inb(port) & ~(1 << IRQline);
	IO::outb(port, value);        
}


void PIC::sendEOI( uint8_t irqno )
{
	// kassert(pic_enabled);
	if (irqno >= 8)
		IO::outb(PIC2_CMD, 0x20);
	IO::outb(PIC1_CMD, 0x20);
}

