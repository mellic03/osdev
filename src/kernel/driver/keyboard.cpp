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
    static uint8_t mod = 0;

    uint8_t code = IO::inb(0x60);

    switch (code)
    {
        default: break;
        case DOWN_LSHIFT:   mod |=  MODIFIER_SHIFT;  break;
        case UP_LSHIFT:     mod &= ~MODIFIER_SHIFT;  break;
        case DOWN_LCTRL:    mod |=  MODIFIER_LCTRL;  break;
        case UP_LCTRL:      mod &= ~MODIFIER_LCTRL;  break;
        case DOWN_LALT:     mod |=  MODIFIER_LALT;   break;
        case UP_LALT:       mod &= ~MODIFIER_LALT;   break;
    }

    struct {
        uint8_t code;
        uint8_t modifier;
    } packet;

    {
        packet = { mod, code };
        KFile_write(KFS::kdevscn, &packet, sizeof(packet));
    }

    PIC::sendEOI(1);
}


char scode_getalpha( uint8_t code )
{
    switch (code)
    {
        default: return '\0';
        case DOWN_Q: return 'Q';
        case DOWN_W: return 'W';
        case DOWN_E: return 'E';
        case DOWN_R: return 'R';
        case DOWN_T: return 'T';
        case DOWN_Y: return 'Y';
        case DOWN_U: return 'U';
        case DOWN_I: return 'I';
        case DOWN_O: return 'O';
        case DOWN_P: return 'P';
        case DOWN_A: return 'A';
        case DOWN_S: return 'S';
        case DOWN_D: return 'D';
        case DOWN_F: return 'F';
        case DOWN_G: return 'G';
        case DOWN_H: return 'H';
        case DOWN_J: return 'J';
        case DOWN_K: return 'K';
        case DOWN_L: return 'L';
        case DOWN_Z: return 'Z';
        case DOWN_X: return 'X';
        case DOWN_C: return 'C';
        case DOWN_V: return 'V';
        case DOWN_B: return 'B';
        case DOWN_N: return 'N';
        case DOWN_M: return 'M';
    }
}



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

