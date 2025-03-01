#include "ckterminal.hpp"
#include "terminal.hpp"
#include <stdc++/cstdio.hpp>
#include <stdc++/cstring.hpp>
#include <stdc++/algorithm.hpp>

#include "../../drivers/serial.hpp"


static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}



static ckTerminal *instance;


int ree( char c )
{
	instance->putchar(c);
	instance->flush();
	return c;
}

int ree2( const char *str )
{
	instance->putstr(str);
	instance->flush();
	return 1;
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

	m_view 	    = {0, 0};
	m_lastflush = {-1, -1};

	m_buf.fill(0, bufw*bufh, vga_entry(' ', m_color));

	instance = this;

	std::cstdio_init((void*)(&ree), (void*)(&ree2));

}


void ckTerminal::flush()
{
	if (m_VGA.data() == m_buf.data())
	{
		return;
	}

	int vga_row = 0;
	int buf_row = getrow();
	auto color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);


	for (int i=0; i<SZ; i++)
	{
		m_VGA.data()[i] = vga_entry(' ', color);
	}

	// std::memset(m_VGA.data(), 0, m_VGA.SZ*sizeof(uint16_t));

	while (vga_row < m_VGA.H && buf_row < m_buf.H)
	{
		// uint16_t *dst = m_VGA.data() + vga_row*m_VGA.W;
		// uint16_t *src = m_buf.data() + buf_row*m_buf.W;

		for (int c=0; c<W; c++)
		{
			m_VGA.data()[vga_row*m_VGA.W + c] = m_buf.data()[buf_row*m_buf.W + c];
		}
		// std::memcpy(dst+vga_row, src+buf_row, W*sizeof(uint16_t));

		vga_row += 1;
		buf_row += 1;
	}

}




void
ckTerminal::setcursor( int idx )
{
    CSR = std::clamp(idx, 0, SZ-1);

	// if (idx/m_buf.W >= m_buf.H)
	// {
	// 	CSR = 0;
	// 	_overflow();
	// }

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
	this->movecursor(1);
}


void
ckTerminal::putchar( char c )
{
	if (c == '\t')
	{
		ck::serial::outb(ck::serial::COM1, ' ');
		ck::serial::outb(ck::serial::COM1, ' ');
	}

	else
	{
		ck::serial::outb(ck::serial::COM1, c);
	}



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
}




void
ckTerminal::_overflow()
{
	// for (int c=0; c<m_buf.W; c++)
	// {
	// 	m_buf[0][c] = vga_entry(' ', m_color);
	// 	m_buf[1][c] = vga_entry(' ', m_color);
	// 	m_buf[2][c] = vga_entry(' ', m_color);
	// }

	// ck::serial::writestr("[ckTerminal::_overflow]\n");
}


void
ckTerminal::scrolldown( int n )
{

	if (n)
	{
		
	}

    // int row = getrow();

	// for (int r=0; r<row; r++)
	// {
    //     for (int c=0; c<W; c++)
    //     {
    //         m_buf[r][c] = m_buf[r+1][c];
    //     }

	// 	// uint16_t *dst = m_buf.data() + W*(r+0);
	// 	// uint16_t *src = m_buf.data() + W*(r+1);
	// 	// std::memcpy(dst, src, W);
	// }

	// for (int r=row; r<H; r++)
	// {
    //     for (int c=0; c<W; c++)
    //     {
    //         m_buf[r][c] = 0;
    //     }

	// 	// uint16_t *dst = m_buf.data() + W*row;
	// 	// uint16_t *src = m_buf.data() + W*(row+1);
	// 	// std::memcpy(dst, src, W);
	// }

    // // setrow(row-1);
    // // setcol(0);
}