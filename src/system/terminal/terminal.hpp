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
    static constexpr int MAX_BUFSIZE = 1024;
    const int ID;

    Terminal( int id, int width, int height, char *buf );

    void setcursor( int );
    void setcursor( int, int );
    void movecursor( int );

    int  getrow() { return CSR/W; };
    int  getcol() { return CSR%W; };
    void setrow( int );
    void setcol( int );

    void write( char );

};


