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

#define STATE_NORMAL 0
#define STATE_PREFIX 1



void
kdriver::ps2_kb::irq_handler( kstackframe *frame )
{
    uint8_t code = IO::inb(0x60);
    KFile_write(KFS::kdevraw, &code, 1);
    PIC::sendEOI(1);
}




static char *shift_table;

static void create_shift_table()
{
    shift_table = (char*)kmalloc(256);
    memset(shift_table, '\0', 256);

    char mod_0_9[] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '('};
    memcpy(&(shift_table['0']), mod_0_9, sizeof(mod_0_9));

    for (int i=0; i<='z'-'a'; i++)
    {
        shift_table['a'+i] = toupper('a'+i);
    }

    shift_table['/'] = '?';
    shift_table['['] = '{';
    shift_table[']'] = '}';
    shift_table[';'] = ':';
    shift_table['\''] = '\"';
    shift_table[','] = '<';
    shift_table['.'] = '>';

}



static void
driver_update( uint8_t scancode )
{
    using namespace kdriver::ps2_kb;
    static uint8_t state = STATE_NORMAL;
    static uint8_t mask = 0;

    uint8_t key  = scode_getchar(scancode);

    if (scancode == 0xE0)
    {
        state = STATE_PREFIX;
        return;
    }

    else if (state == STATE_PREFIX)
    {
        switch (scancode)
        {
            default: break;
            case 0x48: case 0xC8: mask |= KeyEvent_U; break;
            case 0x4B: case 0xCB: mask |= KeyEvent_L; break;
            case 0x4D: case 0xCD: mask |= KeyEvent_R; break;
            case 0x50: case 0xD0: mask |= KeyEvent_D; break;
        }
 
        state = STATE_NORMAL;
    }

    else
    {
        if (scancode >= UP_ESC) mask |=  KeyEvent_UP;
        else                    mask &= ~KeyEvent_UP;

        switch (scancode)
        {
            default: break;

            case DOWN_LSHIFT:   mask |=  KeyEvent_SHIFT; break;
            case UP_LSHIFT:     mask &= ~KeyEvent_SHIFT; break;
        
            case DOWN_LCTRL:    mask |=  KeyEvent_CTRL;  break;
            case UP_LCTRL:      mask &= ~KeyEvent_CTRL;  break;
        
            case DOWN_LALT:     mask |=  KeyEvent_ALT;   break;
            case UP_LALT:       mask &= ~KeyEvent_ALT;   break;
        }

        if (mask & KeyEvent_SHIFT)
        {
            key = isalpha(key) ? toupper(key) : shift_table[key];
        }
    }

    KeyEvent event = {
        .mask = mask,
        .key  = key
    };

    // klock_acquire(&KFS::kdevkey->lock);
    KFile_write(KFS::kdevkey, &event, sizeof(KeyEvent));
    // klock_release(&KFS::kdevkey->lock);
}



void
kdriver::ps2_kb::driver_main( void* )
{
    create_shift_table();
    uint8_t scancode;

    while (true)
    {
        if (KFile_read(&scancode, KFS::kdevraw, 1))
        {
            driver_update(scancode);
        }

        kproc_yield();
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

        case DOWN_MINUS:            return '-';
        case DOWN_BACKSPACE:        return 8;
        case DOWN_TAB:              return '\t';
        case DOWN_ENTER:            return '\n';
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

