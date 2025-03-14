#include "./pic.hpp"
#include "../io.hpp"

#define IRQ_BASE 0x20

void
idk::PIC::init()
{
    // ICW1: start initialization, ICW4 needed
    IO::outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    IO::outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);

    // ICW2: interrupt vector address
    IO::outb(PIC1_DATA, IRQ_BASE);
    IO::outb(PIC2_DATA, IRQ_BASE + 8);

    // ICW3: master/slave wiring
    IO::outb(PIC1_DATA, 4);
    IO::outb(PIC2_DATA, 2);

    // ICW4: 8086 mode, not special fully nested, not buffered, normal EOI
    IO::outb(PIC1_DATA, ICW4_8086);
    IO::outb(PIC2_DATA, ICW4_8086);

    // OCW1: Disable all IRQs
    IO::outb(PIC1_DATA, 0xff);
    IO::outb(PIC2_DATA, 0xff);
}


void
idk::PIC::sendEOI()
{
    IO::outb(PIC1_CMD, 0x20);
}


void
idk::PIC::remap( int master_offset, int slave_offset )
{
    using namespace idk::PIC;

	// Save Interrupt Masking register (IMR).
	// IMR is a bitmap of lines going into PIC.
	// If bit of relevant line is set, PIC ignores the IRQ.
	uint8_t master_int_mask, slave_int_mask;
	master_int_mask = idk::IO::inb(PIC1_DATA);
	slave_int_mask = idk::IO::inb(PIC2_DATA);

	// After receiving 0x11 command, PICs will await 3 more bytes:
	//		- The vector offset (ICW1);
	//		- The master/slave configuration (ICW2);
	//		- Environment info.
	idk::IO::outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
	idk::IO::outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
	
	// Set master and slave PIC vector offsets.
	idk::IO::outb(PIC1_DATA, master_offset);
	idk::IO::outb(PIC2_DATA, slave_offset);
	
	// Tell master PIC that there is a slave PIC connected to IRQ 
	// line 2 (0000 0100).	
	idk::IO::outb(PIC1_DATA, 4);
	// Tell slave PIC that it sends through IRQ2 - its 
	// "cascade identity".
	idk::IO::outb(PIC2_DATA, 2);
	
	// I think this tells the PICs that we're using x86 arch?
	idk::IO::outb(PIC1_DATA, ICW4_8086);
	idk::IO::outb(PIC2_DATA, ICW4_8086);
	
	// Rather than restoring saved masks, only receive mouse/kb ints.
	idk::IO::outb(PIC1_DATA, master_int_mask);
	idk::IO::outb(PIC2_DATA, slave_int_mask);
}