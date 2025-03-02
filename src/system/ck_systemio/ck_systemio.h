#pragma once

#include <stddef.h>
#include <stdint.h>
#include "./ck_file.h"


#ifdef __cplusplus
extern "C" {
#endif


void    ck_writeport( uint8_t port, uint8_t byte );
uint8_t ck_readport( uint8_t port );


int  ckSystemIO_init();
bool ckSystemIO_update();


#ifdef __cplusplus
}
#endif

