#pragma once

#include <stddef.h>
#include <stdint.h>
#include "./idk_file.h"
#include "../memory/memory.hpp"


// #ifdef __cplusplus
// extern "C" {
// #endif


// void    idk_writeport( uint8_t port, uint8_t byte );
// uint8_t idk_readport( uint8_t port );

void idk_sysio_init( idk::linear_allocator& );
bool idk_sysio_update();


// #ifdef __cplusplus
// }
// #endif

