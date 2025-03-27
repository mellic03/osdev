#pragma once

#include <kfile.h>


struct kn_TTY
{
private:
    void _shift_left( int r );
    void _shift_right( int r );
    void _insert( char );

public:
    // idk::static_vector<char, 80*25> data;
    char data[25*80];
    int  W, H;
    int  row, col;
    int  CSR;
    bool dirty;
    // KFile *stream;

    kn_TTY();

    void setcursor( int csr );
    void setrow( int r );
    void setcol( int c );

    void movecursor( int dir );
    void moverow( int dir );
    void movecol( int dir );

    void backspace();

    void putchar( char c );

};


void tty_main( void *arg );
void keyprocess_main( void *arg );
