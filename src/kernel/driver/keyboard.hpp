#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kstackframe.h>




void keyboard_irq_handler( kstackframe* );

char scode_getalpha( uint8_t );

