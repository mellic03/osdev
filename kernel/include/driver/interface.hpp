#pragma once

#include <string.h>
#include <kdef.h>
#include <kernel/interrupt.hpp>

using driverEntryFn = void (*)(void*);


struct hwDriverInterface
{
    char          name[32];
    uint8_t       irqno   = 255;
    irqHandlerFn  handler = nullptr;
    driverEntryFn entry   = nullptr;
    void         *arg     = nullptr;

    hwDriverInterface( const char *nme )
    {
        memset(name, '\0', sizeof(name));
        strncpy(name, nme, 32);
    }

    virtual void loadIrqHandler() { };
};

