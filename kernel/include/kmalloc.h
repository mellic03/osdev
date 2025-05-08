#pragma once
#include <stdint.h>

void *kmalloc( size_t size );
void *krealloc( void*, size_t size );
void  kfree( void* );

