#pragma once

#include <stddef.h>
#include <stdint.h>


namespace idk
{
	class Terminal;
}


class idk::Terminal
{
private:
    int W, H, SZ;
	int CSR;
	char *m_buf;

    void _backsp();
    void _newline();
    void _tab();


public:
    Terminal( int width, int height, char *buf );

    void setcursor( int );
    void setcursor( int, int );
    void movecursor( int );

    int  getrow() { return CSR/W; };
    int  getcol() { return CSR%W; };
    void setrow( int );
    void setcol( int );

    void write( char );

};


