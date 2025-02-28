#pragma once

#include <stddef.h>
#include <stdint.h>
#include "ckbuffer.hpp"

class ckTerminal
{
private:
    int W, H, SZ;
    int CSR;
    uint8_t m_color;

    ckBuffer  m_buf;
    uint16_t *m_VGA;

    struct {
        int start, end;
    } m_lastflush;


public:
    ckTerminal( int outw, int outh, uint16_t *addr );
                // int bufw, int bufh );

    void flush();

    void setcursor( int );
    void setcursor( int, int );
    void movecursor( int );

    int  getrow() { return CSR/W; };
    int  getcol() { return CSR%W; };
    void setrow( int );
    void setcol( int );

    void backsp();
    void newline();
    void tab();

    void putchar( char );
    void putstr( const char* );
    void scrolldown();

};

