#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


int  serial_printf( const char *fmt, ... );
int  syslog_pushIndent( int n );
void syslog_popIndent( int n );
void syslog_clearIndent();


#ifdef __cplusplus
}
#endif
