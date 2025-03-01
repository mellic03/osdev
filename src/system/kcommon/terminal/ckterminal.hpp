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

    ckBuffer  m_VGA;
    ckBuffer  m_buf;

    struct {
        int start, end;
    } m_view, m_lastflush;

    void _overflow();

public:
    ckTerminal( int dstw, int dsth, uint16_t *dst,
                int bufw, int bufh, uint16_t *buf );

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
    void scrolldown( int );

};

