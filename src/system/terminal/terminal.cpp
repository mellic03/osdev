#include "terminal.hpp"
#include <stdint.h>
#include <stddef.h>
#include <cstdio.hpp>
#include <algorithm.hpp>

#include "ckterminal.hpp"



static ckTerminal *instance;

int ckTerminalInit( ckTerminal &term )
{
	instance = &term;

	return 0;
}


// void terminal_setcolor(uint8_t color) 
// {
// 	instance->color = color;
// }

// void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
// {
// 	const size_t index = y * instance->VGA_WIDTH + x;
// 	instance->buf[index] = vga_entry(c, color);
// }

