#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


void SYSLOG_BEGIN( const char* );
void SYSLOG( const char *fmt, ... );
void SYSLOG_END();
#define SYSLOG_RET(_r) SYSLOG_END(); return _r


#ifdef __cplusplus
}
#endif


