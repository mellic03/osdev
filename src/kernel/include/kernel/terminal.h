#pragma once

#ifdef __cplusplus
extern "C" {
#endif


void terminal_initialize( void );
void terminal_putc( char );
void terminal_puts( const char* );


#ifdef __cplusplus
}
#endif
