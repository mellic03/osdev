#pragma once

#include <kfile.h>


struct kn_TTY
{
    // idk::static_vector<char, 80*25> data;
    char data[25][80];
    int  row, col;
    int  csr;
    // KFile *stream;

    kn_TTY();

    void putchar( char c );
    // void update();

};


