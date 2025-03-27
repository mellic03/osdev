#pragma once

#include <kfile.h>



struct kn_TTY
{
private:
    void _shift_left();
    void _shift_right();
    void _insert( char );

public:
    // idk::static_vector<char, 80*25> data;
    char data[25*80];
    char *data_top;
    char *data_end;
    int  W, H;
    bool dirty;

    char prompt[80];
    int  prompt_csr;
    // KFile *stream;

    kn_TTY();

    // void setcursor( int csr );
    // void setrow( int r );
    // void setcol( int c );

    void movecursor( int dir );
    // void moverow( int dir );
    // void movecol( int dir );

    void backspace();
    void enter();

    void putchar( char c );

};




// void tty_main( void *arg );
void prompt_main( void *arg );
void keyprocess_main( void *arg );
