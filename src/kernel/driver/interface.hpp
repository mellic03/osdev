#pragma once

#include <string.h>
#include <kdef.h>
#include <kstackframe.h>

struct hwDriverInterface
{
    using irqh_type  = void (*)(kstackframe*);
    using entry_type = void (*)(void*);

    char       name[32];
    uint8_t    irqno   = 255;
    irqh_type  handler = nullptr;
    entry_type entry   = nullptr;

    hwDriverInterface( const char *nme )
    {
        memset(name, '\0', sizeof(name));
        strncpy(name, nme, 32);
    }

    virtual void loadIrqHandler() { };
};

