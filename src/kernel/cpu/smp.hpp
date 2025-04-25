#pragma once

struct limine_mp_response;

namespace SMP
{
    void init( limine_mp_response* );
    uint32_t get_lapic_id();
}

