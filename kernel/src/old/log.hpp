#pragma once
#include "drivers/serial.h"

#define SYSLOG serial_printf

#define SYSLOG_BEGIN(__fn_name__)\
    SYSLOG("[" __fn_name__ "]\n");\
    SYSLOG("{\n");\
    syslog_pushIndent(4);\

#define SYSLOG_END \
    syslog_popIndent(4);\
    SYSLOG("}\n");\

