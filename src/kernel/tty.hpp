#pragma once
#include <kfile.h>
#include "kfs/kfs.hpp"


struct kTTY
{
    char *history;
    char *htop, *hend;

    char *prompt;
    char *ptop, *pend;

    vfsDirEntry *cwd;

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

    // const char *submit();

};


void kshell_main( void *arg );

