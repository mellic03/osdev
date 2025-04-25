#pragma once
#include <kfile.h>
#include <stdio.h>
#include <kernel/vfs.hpp>
#include "sde/font.hpp"


struct kTTY
{
private:
    vfsDirEntry *cwd;
    void _putr();

public:
    int hlen;
    int hrow;
    size_t size;

    char *history,  *hbase, *htop, *hend;
    char *prompt,   *pbase, *ptop, *pend;
    char *response, *rbase, *rtop, *rend;

    sde::Font *font;
    bool running;

    kTTY();
    kTTY( int history_length );

    void _clear( char *&top, char *base );
    void hclear() { _clear(htop, hbase); };
    void pclear() { _clear(ptop, pbase); };
    void rclear() { _clear(rtop, rbase); };
    void clear();

    void submit( void (*callback)(kTTY*));
    void _putc( char *&top, char *base, char *end, char ch );
    void hputc( char ch ); // { _putc(htop, history, hend, ch); };
    void pputc( char ch ) { _putc(ptop, prompt, pend, ch); };

    void _puts( char *&top, char *base, char *end, const char *str );
    void hputs( const char *str ); // { _puts(htop, history, hend, str); }
    void pputs( const char *str ) { _puts(ptop, prompt, pend, str); }

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

