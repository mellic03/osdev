#pragma once

#include <kdef.h>
#include <kstackframe.h>
#include <kmath/vec.hpp>

extern uint8_t MousePointer[];

void InitPS2Mouse();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();


extern bool mouseleft;
extern bool mouseright;
extern vec2 mousexy;
extern vec2 mousedelta;

void mouse_irq( kstackframe* );
void mouse_init();
