#pragma once

#include "tty.hpp"


struct KShellCommand
{
    const char *prompt;
};


// void kshell_main( void* );
// Returns new dst pointer
// char *kshell_interpret( char *dst, const char *src, kTTY *tty );
void kshell_interpret( kTTY *tty );
