#include "keyboard_ps2_poll.hpp"
#include "io.hpp"
#include "log.hpp"
#include <string.h>

using namespace idk;

static constexpr uint8_t PORT_DATA    = 0x60; 
static constexpr uint8_t PORT_STATUS  = 0x64; 
static constexpr uint8_t PORT_COMMAND = 0x64;



keyboard_ps2_poll::keyboard_ps2_poll()
{
    memset(state, 0, sizeof(state));
}



// void write_state( uint8_t state[32], uint8_t idx256, bool value )
// {
//     uint8_t idx32 = idx256/8;
//     uint8_t bit   = idx256%8;

//     if (value) state[idx32] |= (1 << bit);
//     else       state[idx32] &= ~(1 << bit);
// }


// bool
// keyboard_ps2_poll::pressed( uint8_t idx256 )
// {
//     uint8_t idx32 = idx256/8;
//     uint8_t bit   = idx256%8;
//     return state[idx32] & (1 << bit);
// }


uint8_t lookup[2556];

void init_lookup()
{
    memset(lookup, 0, sizeof(lookup));

    lookup[0x10] = 'q';
    lookup[0x11] = 'w';
    lookup[0x12] = 'e';
    lookup[0x13] = 'r';
    lookup[0x14] = 't';
    lookup[0x15] = 'y';
    lookup[0x16] = 'u';
    lookup[0x17] = 'i';
    lookup[0x18] = 'o';
    lookup[0x19] = 'p';

    lookup[0x1E] = 'a';
    lookup[0x1F] = 's';
    lookup[0x20] = 'd';
    lookup[0x21] = 'f';
    lookup[0x22] = 'g';
    lookup[0x23] = 'h';
    lookup[0x24] = 'j';
    lookup[0x25] = 'k';
    lookup[0x26] = 'l';

    lookup[0x2C] = 'z';
    lookup[0x2D] = 'x';
    lookup[0x2E] = 'c';
    lookup[0x2F] = 'v';
    lookup[0x30] = 'b';
    lookup[0x31] = 'n';
    lookup[0x32] = 'm';


    lookup[0x90] = 'q';
    lookup[0x91] = 'w';
    lookup[0x92] = 'e';
    lookup[0x93] = 'r';
    lookup[0x94] = 't';
    lookup[0x95] = 'y';
    lookup[0x96] = 'u';
    lookup[0x97] = 'i';
    lookup[0x98] = 'o';
    lookup[0x99] = 'p';

    lookup[0x9E] = 'a';
    lookup[0x9F] = 's';
    lookup[0xA0] = 'd';
    lookup[0xA1] = 'f';
    lookup[0xA2] = 'g';
    lookup[0xA3] = 'h';
    lookup[0xA4] = 'j';
    lookup[0xA5] = 'k';
    lookup[0xA6] = 'l';

    lookup[0xAC] = 'z';
    lookup[0xAD] = 'x';
    lookup[0xAE] = 'c';
    lookup[0xAF] = 'v';
    lookup[0xB0] = 'b';
    lookup[0xB1] = 'n';
    lookup[0xB2] = 'm';
}


uint8_t
keyboard_ps2_poll::poll()
{
    static bool first = true;
    if (first)
    {
        init_lookup();
        first = false;
    }

    uint8_t opcode = IO::inb(PORT_DATA);
    uint8_t value  = lookup[opcode];

    // press
    if (opcode < 0x81)
    {
        state[value] = true;
        return value;
    }

    // release
    else
    {
        state[value] = false;
        return '\0';
    }


}