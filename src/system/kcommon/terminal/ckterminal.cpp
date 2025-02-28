#include "ckterminal.hpp"
#include "terminal.hpp"
#include <stdc++/cstring.hpp>
#include <stdc++/algorithm.hpp>

#include "../serial.hpp"

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}


ckTerminal::ckTerminal( int dstw, int dsth, uint16_t *dst,
						int bufw, int bufh, uint16_t *buf )
:	m_VGA(dstw, dsth, dst),
 	m_buf(bufw, bufh, buf)
{
	W   = bufw;
	H   = bufh;
	SZ  = bufw*bufh;
	CSR = 0;

	m_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	m_lastflush = {-1, -1};
	m_buf.fill(0, bufw*bufh, vga_entry(' ', m_color));
}


void ckTerminal::flush()
{
	if (m_VGA.data() == m_buf.data())
	{
		return;
	}

    std::memcpy(m_VGA.data(), m_buf.data(), SZ*sizeof(uint16_t));
}




void
ckTerminal::setcursor( int idx )
{
	// if (idx)
	CSR = std::clamp(idx, 0, SZ);
}


void
ckTerminal::setcursor( int row, int col )
{
	this->setcursor(row*W + col);
}


void
ckTerminal::movecursor( int n )
{
	this->setcursor(CSR+n);
}


void
ckTerminal::setrow( int row )
{
	int col = getcol();
	setcursor(row*W + col);
}


void
ckTerminal::setcol( int col )
{
	int row = getrow();
	setcursor(row*W + col);
}


void
ckTerminal::backsp()
{
	this->movecursor(-1);
	this->putchar(' ');
	this->movecursor(-1);
}


void
ckTerminal::newline()
{
	this->setrow(this->getrow() + 1);
	this->setcol(0);
}


void
ckTerminal::tab()
{
	this->movecursor(4);
}


void
ckTerminal::putchar( char c )
{
	switch (c)
	{
		default: 					break;
		case '\t': this->tab(); 	return;
		case '\n': this->newline(); return;
	}

	m_buf.write(CSR, vga_entry(c, m_color));
	this->movecursor(1);
}


void
ckTerminal::putstr( const char *str )
{
	while (*str)
    {
		this->putchar(*str);
        str++;
    }

    ck::serial::writemsg(str);
}


void
ckTerminal::scrolldown()
{
    int row = getrow();

	for (int r=0; r<row; r++)
	{
        for (int c=0; c<W; c++)
        {
            m_buf[r][c] = m_buf[r+1][c];
        }

		// uint16_t *dst = m_buf.data() + W*(r+0);
		// uint16_t *src = m_buf.data() + W*(r+1);
		// std::memcpy(dst, src, W);
	}

	for (int r=row; r<H; r++)
	{
        for (int c=0; c<W; c++)
        {
            m_buf[r][c] = 0;
        }

		// uint16_t *dst = m_buf.data() + W*row;
		// uint16_t *src = m_buf.data() + W*(row+1);
		// std::memcpy(dst, src, W);
	}

    // setrow(row-1);
    // setcol(0);
}