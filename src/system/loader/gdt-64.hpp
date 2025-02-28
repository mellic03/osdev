#pragma once 
#include <stddef.h>
#include <stdint.h>



namespace ck
{
    void create_descriptor( uint32_t base, uint32_t limit, uint16_t flag );
    void create_GDT();
}

