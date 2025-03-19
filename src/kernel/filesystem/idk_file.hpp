#pragma once

#include <kfile.h>
#include <kinplace/inplace_ringbuffer.hpp>


struct idk_FILE
{
    uint32_t  fd;
    uint32_t  flags;
    uint32_t  status;
    idk::inplace_ringbuffer<uint8_t> buf;

    idk_FILE();
    idk_FILE( uint8_t *start, uint8_t *end, uint32_t fflags );
};
