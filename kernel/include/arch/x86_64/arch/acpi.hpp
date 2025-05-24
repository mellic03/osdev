#pragma once
#include <stdint.h>

namespace ACPI
{
    struct Response
    {
        uint64_t ioapic_base;
    };

    void init( uintptr_t acpi_phys, ACPI::Response& );
}
