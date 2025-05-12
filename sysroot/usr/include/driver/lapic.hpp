#pragma once
#include <stddef.h>
#include <stdint.h>

namespace LAPIC
{
    void enable();
    extern uint8_t *localAddress;
}

uint32_t LocalApicGetId();
void LocalApicSendInit(uint32_t apic_id);
void LocalApicSendStartup(uint32_t apic_id, uint32_t vector);
