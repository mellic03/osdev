#pragma once
#include <kfile.h>
#include <stdio.h>
#include <kernel/vfs.hpp>
#include "kvideo/font.hpp"


struct kTTY
{
private:
    vfsDirEntry *cwd;

public:
    char *history;
    char *htop, *hend;

    char *prompt;
    char *ptop, *pend;

    idk::FontBuffer *font;
    bool running;

    kTTY();
    kTTY( size_t size );

    void hclear();
    void pclear();
    void clear();

    void _putc( char *&top, char *end, char ch );
    void hputc( char ch ) { _putc(htop, hend, ch); };
    void pputc( char ch ) { _putc(ptop, pend, ch); };

    void _puts( char *&top, char *end, const char *str );
    void hputs( const char *str ) { _puts(htop, hend, str); }
    void pputs( const char *str ) { _puts(ptop, pend, str); }

    template <typename... Args>
    void hsprintf( const char *fmt, Args... args )
    {
        htop += sprintf(htop, fmt, args...);
    }

    template <typename... Args>
    void psprintf( const char *fmt, Args... args )
    {
        ptop += sprintf(ptop, fmt, args...);
    }

    void moveCursor( int dir );

    vfsDirEntry *&getCWD();
    // const char *submit();

};

