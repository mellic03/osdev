#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


typedef struct
{
    size_t    VGA_WIDTH; // = 80;
    size_t    VGA_HEIGHT; // = 25;
    size_t    row, col;
    uint32_t  indent;
    uint8_t   color;
    uint16_t *buf;

} ckTerminal_t;


int  ckTerminalInit( ckTerminal_t* );
void ckTerminal_putc( char );
void ckTerminal_puts( const char* );
void ckTerminal_pushIndent( int n );
void ckTerminal_popIndent( int n );


#ifdef __cplusplus
}
#endif
