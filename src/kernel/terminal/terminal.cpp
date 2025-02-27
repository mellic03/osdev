#include "terminal.h"
#include <stdint.h>
#include <stddef.h>



/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t vga_entry(unsigned char uc, uint8_t color);


static ckTerminal_t *instance;

int ckTerminalInit( ckTerminal_t *term )
{
	instance = term;

	term->row = 0;
	term->col = 0;
	term->color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	term->buf = (uint16_t*) 0xB8000;

	for (size_t y=0; y<term->VGA_HEIGHT; y++)
	{
		for (size_t x=0; x<term->VGA_WIDTH; x++)
		{
			const size_t index = y * term->VGA_WIDTH + x;
			term->buf[index] = vga_entry(' ', term->color);
		}
	}

	return 0;
}





static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}


void terminal_setcolor(uint8_t color) 
{
	instance->color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * instance->VGA_WIDTH + x;
	instance->buf[index] = vga_entry(c, color);
}




void ckTerminal_scrdown( ckTerminal_t *term )
{
	for (size_t i=0; i<term->VGA_WIDTH; i++)
	{
		for (size_t j=0; j<term->VGA_HEIGHT-1; j++)
		{
			size_t idx0 = (j+0)*term->VGA_WIDTH + i;
			size_t idx1 = (j+1)*term->VGA_WIDTH + i;
			term->buf[idx0] = term->buf[idx1];
		}
	}

	for (size_t i=0; i<term->VGA_WIDTH; i++)
	{
		size_t j = term->VGA_HEIGHT-1;
		size_t idx = j*term->VGA_WIDTH + i;
		term->buf[idx] = vga_entry(' ', term->color);
	}
}


void
ckTerminal_putc( char c )
{
	size_t &row = instance->row;
	size_t &col = instance->col;

	if (c == '\t')
	{
		col += 4;
		c = ' ';
	}

	else if (c == '\n')
	{
		col = 0;
		row += 1;
		return;
	}




	if (col >= instance->VGA_WIDTH)
	{
		col = 0;
		row += 1;
	}

	if (row >= instance->VGA_HEIGHT)
	{
		ckTerminal_scrdown(instance);
		row -= 1;
	}

	size_t idx =  row*instance->VGA_WIDTH + col;
	instance->buf[idx] = vga_entry(c, instance->color);

	col += 1;
}


void
ckTerminal_puts( const char *str )
{
	while (*str)
    {
        ckTerminal_putc(*str);
        str++;
    }
}


