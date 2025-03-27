#include "keyboard.hpp"
#include "serial.hpp"
#include "pic.hpp"
#include "../kfs/kfs.hpp"

#include <kscancode.h>
#include <kmalloc.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

using namespace idk;


void keyboard_irq_handler( kstackframe *frame )
{
    static bool multi = false;
    static uint8_t mod = 0;
    uint8_t code = IO::inb(0x60);

    if (code == 0xE0)
    {
        multi = true;
        PIC::sendEOI(1);
        return;
    }

    if (multi)
    {
        multi = false;

        // if (code == 0x6B)
        //     code = DOWN_LEFT;
        // if (code == 0x74)
        //     code = DOWN_RIGHT;
        
        mod |= MODIFIER_CURSOR;
    }
    else
    {
        mod &= ~MODIFIER_CURSOR;
    }


    switch (code)
    {
        default: break;
        case DOWN_LSHIFT:   mod |=  MODIFIER_SHIFT;  break;
        case UP_LSHIFT:     mod &= ~MODIFIER_SHIFT;  break;
        case DOWN_LCTRL:    mod |=  MODIFIER_CTRL;   break;
        case UP_LCTRL:      mod &= ~MODIFIER_CTRL;   break;
        case DOWN_LALT:     mod |=  MODIFIER_ALT;    break;
        case UP_LALT:       mod &= ~MODIFIER_ALT;    break;
    }

    struct {
        uint8_t modifier;
        uint8_t code;
    } packet;

    {
        packet = { mod, code};
        KFile_write(KFS::kdevscn, &packet, sizeof(packet));
    }

    PIC::sendEOI(1);
}




// void
// kdriver::ps2_kb::driver_main( void *arg )
// {
//     KFile *fh = (KFile*)arg;
//     Packet packet;

//     while (true)
//     {
//         packet = {0, 0};

//         if (KFile_read(&packet, fh, sizeof(packet)))
//         {
//             if (packet.modifier == MODIFIER_CURSOR)
//             {
//                 if (packet.code == 0x4B)
//                 {
//                     tty.movecursor(-1);
//                 }
//                 if (packet.code == 0x4D)
//                 {
//                     tty.movecursor(+1);
//                 }

//                 continue;
//             }

//             char ch = scode_getchar(packet.code);
    
//             if (ch == '\0')
//             {
//                 continue;
//             }

//             if (packet.modifier & MODIFIER_SHIFT)
//             {
//                 if (shift_table[ch])
//                 {
//                     ch = shift_table[ch];
//                 }
//             }

//             KFile_write(KFS::kdevkey, &ch, 1);
//         }

//         // printf("[keyprocess_main]\n");
//         kproc_yield();
//     }

// }





char scode_getchar( uint8_t code )
{
    if (code == DOWN_0)
    {
        return '0';
    }

    if (DOWN_1 <= code && code <= DOWN_9)
    {
        return char('0' + code - 1);
    }

    switch (code)
    {
        default: return '\0';
        case DOWN_Q: return 'q';
        case DOWN_W: return 'w';
        case DOWN_E: return 'e';
        case DOWN_R: return 'r';
        case DOWN_T: return 't';
        case DOWN_Y: return 'y';
        case DOWN_U: return 'u';
        case DOWN_I: return 'i';
        case DOWN_O: return 'o';
        case DOWN_P: return 'p';
        case DOWN_A: return 'a';
        case DOWN_S: return 's';
        case DOWN_D: return 'd';
        case DOWN_F: return 'f';
        case DOWN_G: return 'g';
        case DOWN_H: return 'h';
        case DOWN_J: return 'j';
        case DOWN_K: return 'k';
        case DOWN_L: return 'l';
        case DOWN_Z: return 'z';
        case DOWN_X: return 'x';
        case DOWN_C: return 'c';
        case DOWN_V: return 'v';
        case DOWN_B: return 'b';
        case DOWN_N: return 'n';
        case DOWN_M: return 'm';

        case DOWN_BACKSPACE:    return 8;
        case DOWN_TAB:          return '\t';

        case DOWN_ENTER: return '\n';
        case DOWN_LEFT_BRACKET:     return '(';
        case DOWN_RIGHT_BRACKET:    return ')';
        case DOWN_SPACE:            return ' ';
        case DOWN_SEMICOLON:        return ';';
        case DOWN_SINGLE_QUOTE:     return '\'';
        case DOWN_BACKSLASH:        return '\\';
        case DOWN_COMMA:            return ',';
        case DOWN_PERIOD:           return '.';
        case DOWN_FORWARDSLASH:     return '/';
    }
}

