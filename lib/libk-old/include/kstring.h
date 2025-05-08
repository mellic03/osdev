#pragma once
#include "AAA.h"


const char *skip_ch( const char *s, char ch );
const char *seek_ch( const char *s, char ch );
const char *skip_brk( const char *s, const char *brk );
const char *seek_brk( const char *s, const char *brk );

size_t count_ch( const char *s, char ch );


#include "ZZZ.h"
