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
	char *m_buf;

    void _newline();
    void _tab();


public:
    static constexpr int MAX_BUFSIZE = 1024;
    const int ID;
    int W, H, SZ;
	int CSR;

    Terminal( int id, int width, int height, char *buf );

    const char *data() { return m_buf; };

    void setcursor( int );
    void setcursor( int, int );
    void movecursor( int );

    int  getrow() { return CSR/W; };
    int  getcol() { return CSR%W; };
    int  getrow( int csr ) { return csr/W; };
    int  getcol( int csr ) { return csr%W; };
    void setrow( int );
    void setcol( int );

    void putchar( char );
    void putchar_at( int row, int col, char );

    void putstr( const char* );
    void putstr_at( int row, int col, const char* );

};


