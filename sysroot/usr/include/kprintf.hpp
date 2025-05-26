#pragma once
#include <kernel/tty.hpp>
#include <gui/gui.hpp>

void kprintf_init( knlTTY *tty, guiTextArea* );
void kprintf_redraw( void* );
void kprintf( const char*, ... );