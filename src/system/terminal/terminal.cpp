#include "./terminal.hpp"
#include "system/drivers/serial.hpp"
#include "system/memory/memory.hpp"
#include <cstdio.hpp>
#include <cstring.hpp>
#include <algorithm.hpp>


idk::Terminal::Terminal( int id, int width, int height, char *buf )
:	ID(id)
{
	W   = width;
	H   = height;
	SZ  = width*height;
	CSR = 0;
	m_buf = buf;
}



void
idk::Terminal::setcursor( int idx )
{
    CSR = std::clamp(idx, 0, SZ-1);
}


void
idk::Terminal::setcursor( int row, int col )
{
	setcursor(row*W + col);
}


void
idk::Terminal::movecursor( int n )
{
	setcursor(CSR+n);
}


void
idk::Terminal::setrow( int row )
{
	int col = getcol();
	setcursor(row*W + col);
}


void
idk::Terminal::setcol( int col )
{
	int row = getrow();
	setcursor(row*W + col);
}


void
idk::Terminal::_backsp()
{
	movecursor(-1);
	write(' ');
	movecursor(-1);
}


void
idk::Terminal::_newline()
{
	setrow(getrow() + 1);
	setcol(0);
}


void
idk::Terminal::_tab()
{
	movecursor(1);
}


void
idk::Terminal::write( char c )
{
	switch (c)
	{
		default:				break;
		case '\t': _tab(); 		return;
		case '\n': _newline(); 	return;
	}

	m_buf[CSR] = c;
	movecursor(1);
}

