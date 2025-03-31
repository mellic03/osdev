#pragma once

#include <kdef.h>
#include <kstackframe.h>
#include <kfstream.hpp>


struct hwDriverInterface
{
    uint8_t irqno;

    void (*irq_handler)(kstackframe*);
    void (*driver_entry)(void*);
};


