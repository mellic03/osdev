#pragma once

struct knlTTY
{
    char buf[1024];
    int idx;
    // void (*flush)(void*);

    knlTTY();
    void clear();
    void putch( char ch );
    void putstr( const char *str );

};


