#pragma once
#include "AAA.h"


// enum PS2_MODIFIER: uint8_t
// {
//     MODIFIER_SHIFT = 1 << 0,

//     MODIFIER_CTRL  = 1 << 1,
//     MODIFIER_ALT   = 1 << 2,

//     MODIFIER_CURSOR  = 1 << 3
// };

enum PS2_SCANCODE: uint8_t
{
    DOWN_ESC = 0x01, // 0x01 	
    DOWN_1,          // 0x02 	
    DOWN_2,          // 0x03 	
    DOWN_3,          // 0x04 	
    DOWN_4,          // 0x05 	
    DOWN_5,          // 0x06 	
    DOWN_6,          // 0x07 	
    DOWN_7,          // 0x08 	
    DOWN_8,          // 0x09 	
    DOWN_9,          // 0x0A 	
    DOWN_0,          // 0x0B 	
    DOWN_MINUS,      // 0x0C 	
    DOWN_EQUAL,      // 0x0D 	
    DOWN_BACKSPACE,  // 0x0E 	
    DOWN_TAB,        // 0x0F 	
    DOWN_Q,          // 0x10 	
    DOWN_W,          // 0x11 	
    DOWN_E,          // 0x12 	
    DOWN_R,          // 0x13 	
    DOWN_T,          // 0x14 	
    DOWN_Y,          // 0x15 	
    DOWN_U,          // 0x16 	
    DOWN_I,          // 0x17 	
    DOWN_O,          // 0x18 	
    DOWN_P,          // 0x19 	
    DOWN_LEFT_BRACKET,   // 0x1A 	
    DOWN_RIGHT_BRACKET,  // 0x1B 	
    DOWN_ENTER,          // 0x1C 	
    DOWN_LCTRL,          // 0x1D 	
    DOWN_A,          // 0x1E 	
    DOWN_S,          // 0x1F 	
    DOWN_D,          // 0x20 	
    DOWN_F,          // 0x21 	
    DOWN_G,          // 0x22 	
    DOWN_H,          // 0x23 	
    DOWN_J,          // 0x24 	
    DOWN_K,          // 0x25 	
    DOWN_L,          // 0x26 	
    DOWN_SEMICOLON,     // 0x27 	
    DOWN_SINGLE_QUOTE,  // 0x28 	
    DOWN_BACKTICK,      // 0x29 	
    DOWN_LSHIFT,        // 0x2A 	
    DOWN_BACKSLASH,     // 0x2B 	
    DOWN_Z,          // 0x2C 	
    DOWN_X,          // 0x2D 	
    DOWN_C,          // 0x2E 	
    DOWN_V,          // 0x2F 	
    DOWN_B,          // 0x30 	
    DOWN_N,          // 0x31 	
    DOWN_M,          // 0x32 	
    DOWN_COMMA,         // 0x33 	
    DOWN_PERIOD,        // 0x34 	
    DOWN_FORWARDSLASH,  // 0x35 	
    DOWN_RSHIFT,        // 0x36 	
    DOWN_KEYPAD_STAR,   // 0x37 	
    DOWN_LALT,          // 0x38 	
    DOWN_SPACE,         // 0x39 	
    DOWN_CAPS,          // 0x3A 	
    DOWN_F1,         // 0x3B 	
    DOWN_F2,         // 0x3C 	
    DOWN_F3,         // 0x3D 	
    DOWN_F4,         // 0x3E 	
    DOWN_F5,         // 0x3F 	
    DOWN_F6,         // 0x40 	
    DOWN_F7,         // 0x41 	
    DOWN_F8,         // 0x42 	
    DOWN_F9,         // 0x43 	
    DOWN_F10,            // 0x44 	
    // 0x45 	NumberLock pressed
    // 0x46 	ScrollLock pressed
    // 0x47 	(keypad) 7 pressed
    // 0x48 	(keypad) 8 pressed
    // 0x49 	(keypad) 9 pressed
    // 0x4A 	(keypad) - pressed
    // 0x4B 	(keypad) 4 pressed
    // 0x4C 	(keypad) 5 pressed
    // 0x4D 	(keypad) 6 pressed
    // 0x4E 	(keypad) + pressed
    // 0x4F 	(keypad) 1 pressed
    // 0x50 	(keypad) 2 pressed
    // 0x51 	(keypad) 3 pressed
    // 0x52 	(keypad) 0 pressed
    // 0x53 	(keypad) . pressed
    // 0x57 	F11 pressed 
    // 0x58 	F12 pressed 

    UP_ESC = 0x81, // 0x01 	
    UP_1,          // 0x02 	
    UP_2,          // 0x03 	
    UP_3,          // 0x04 	
    UP_4,          // 0x05 	
    UP_5,          // 0x06 	
    UP_6,          // 0x07 	
    UP_7,          // 0x08 	
    UP_8,          // 0x09 	
    UP_9,          // 0x0A 	
    UP_0,          // 0x0B 	
    UP_MINUS,      // 0x0C 	
    UP_EQUAL,      // 0x0D 	
    UP_BACKSPACE,  // 0x0E 	
    UP_TAB,        // 0x0F 	
    UP_Q,          // 0x10 	
    UP_W,          // 0x11 	
    UP_E,          // 0x12 	
    UP_R,          // 0x13 	
    UP_T,          // 0x14 	
    UP_Y,          // 0x15 	
    UP_U,          // 0x16 	
    UP_I,          // 0x17 	
    UP_O,          // 0x18 	
    UP_P,          // 0x19 	
    UP_LEFT_BRACKET,   // 0x1A 	
    UP_RIGHT_BRACKET,  // 0x1B 	
    UP_ENTER,          // 0x1C 	
    UP_LCTRL,          // 0x1D 	
    UP_A,          // 0x1E 	
    UP_S,          // 0x1F 	
    UP_D,          // 0x20 	
    UP_F,          // 0x21 	
    UP_G,          // 0x22 	
    UP_H,          // 0x23 	
    UP_J,          // 0x24 	
    UP_K,          // 0x25 	
    UP_L,          // 0x26 	
    UP_SEMICOLON,     // 0x27 	
    UP_SINGLE_QUOTE,  // 0x28 	
    UP_BACKTICK,      // 0x29 	
    UP_LSHIFT,        // 0x2A 	
    UP_BACKSLASH,     // 0x2B 	
    UP_Z,          // 0x2C 	
    UP_X,          // 0x2D 	
    UP_C,          // 0x2E 	
    UP_V,          // 0x2F 	
    UP_B,          // 0x30 	
    UP_N,          // 0x31 	
    UP_M,          // 0x32 	
    UP_COMMA,         // 0x33 	
    UP_PERIOD,        // 0x34 	
    UP_FORWARDSLASH,  // 0x35 	
    UP_RSHIFT,        // 0x36 	
    UP_KEYPAD_STAR,   // 0x37 	
    UP_LALT,          // 0x38 	
    UP_SPACE,         // 0x39 	
    UP_CAPS,          // 0x3A 	
    UP_F1,         // 0x3B 	
    UP_F2,         // 0x3C 	
    UP_F3,         // 0x3D 	
    UP_F4,         // 0x3E 	
    UP_F5,         // 0x3F 	
    UP_F6,         // 0x40 	
    UP_F7,         // 0x41 	
    UP_F8,         // 0x42 	
    UP_F9,         // 0x43 	
    UP_F10,            // 0x44 	
    // 0x45 	NumberLock pressed
    // 0x46 	ScrollLock pressed
    // 0x47 	(keypad) 7 pressed
    // 0x48 	(keypad) 8 pressed
    // 0x49 	(keypad) 9 pressed
    // 0x4A 	(keypad) - pressed
    // 0x4B 	(keypad) 4 pressed
    // 0x4C 	(keypad) 5 pressed
    // 0x4D 	(keypad) 6 pressed
    // 0x4E 	(keypad) + pressed
    // 0x4F 	(keypad) 1 pressed
    // 0x50 	(keypad) 2 pressed
    // 0x51 	(keypad) 3 pressed
    // 0x52 	(keypad) 0 pressed
    // 0x53 	(keypad) . pressed
    // 0x57 	F11 pressed 
    // 0x58 	F12 pressed 

    DOWN_LEFT,
    DOWN_RIGHT
};


#include "ZZZ.h"
