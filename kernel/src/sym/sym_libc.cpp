#pragma once
#include <sym/sym.hpp>

#include <kernel/log.hpp>
#include <kernel/clock.hpp>
#include <kmalloc.h>

symtable_t sym_libc[]
{
    SYM_BEGIN(),
        SYM_ALIAS("clock",  kclock::now),
        SYM_ALIAS("malloc", kmalloc),
        SYM_ALIAS("printf", syslog::kprintf),
    SYM_END()
};


