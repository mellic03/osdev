#pragma once



struct KShellCommand
{
    const char *prompt;
};


// void kshell_main( void* );
// Returns new dst pointer
char *kshell_interpret( char *dst, const char *src );
