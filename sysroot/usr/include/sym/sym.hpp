#pragma once
// #include <stddef.h>
// #include <stdint.h>
#include <kdef.h>

enum Sym_ ENUM_DTYPE(uint8_t)
{
    Sym_Begin = 1,
    Sym_Entry,
    Sym_End
};

struct symtable_t
{
    uint8_t type;
    char    name[16];
    void   *addr;
};


#define SYM_BEGIN()            {Sym_Begin, "\0",  nullptr}
#define SYM_ENTRY(func)        {Sym_Entry, #func, (void*)func}
#define SYM_ALIAS(alias, func) {Sym_Entry, alias, (void*)func}
#define SYM_END()              {Sym_End,   "\0",  nullptr}

