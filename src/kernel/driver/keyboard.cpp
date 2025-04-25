#ifndef __is_kernel
    #define __is_kernel
#endif

#include "keyboard.hpp"
#include "../interrupt/interrupt.hpp"
#include "pic.hpp"
#include <kernel/ioport.hpp>
#include <kernel/vfs.hpp>
#include <kernel/log.hpp>

#include <kpanic.h>
#include <kscancode.h>
#include <kmalloc.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

using namespace idk;

#define STATE_NORMAL 0
#define STATE_PREFIX 1


static vfsFileEntry *rawfh = nullptr;
static vfsFileEntry *keyfh = nullptr;


void
kdriver::ps2_kb::irq_handler( kstackframe* )
{
    uint8_t code = IO::inb(0x60);

    if (rawfh)
        rawfh->stream.write(&code, 1);
    else
        syslog::kprintf("[ps2_kb::irq_handler] REEEEEE\n");
}




static char *shift_table;

static void create_shift_table()
{
    shift_table = (char*)kmalloc(256);
    memset(shift_table, '\0', 256);

    for (int i=0; i<256; i++)
    {
        shift_table[i] = char(i);
    }

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

            case 0x48:  mask |=  KeyEvent_U; break;
            case 0xC8:  mask &= ~KeyEvent_U; break;

            case 0x4B:  mask |=  KeyEvent_L; break;
            case 0xCB:  mask &= ~KeyEvent_L; break;

            case 0x4D:  mask |=  KeyEvent_R; break;
            case 0xCD:  mask &= ~KeyEvent_R; break;

            case 0x50:  mask |=  KeyEvent_D; break;
            case 0xD0:  mask &= ~KeyEvent_D; break;
        }
 
        KeyEvent event = {
            .mask = mask,
            .key  = 0
        };

        keyfh->stream.write(&event, sizeof(KeyEvent));
    
        state = STATE_NORMAL;
        return;
    }

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

    KeyEvent event = {
        .mask = mask,
        .key  = key
    };

    keyfh->stream.write(&event, sizeof(KeyEvent));
}


void
kdriver::ps2_kb::driver_main( void* )
{
    create_shift_table();

    rawfh = vfsFindFile("/dev/kb0/raw");
    keyfh = vfsFindFile("/dev/kb0/event");
    uint8_t scancode;

    while (true)
    {
        if (rawfh == nullptr)
        {
            kpanic("BitchAss");
        }

        if (rawfh->stream.read(&scancode, 1))
        {
            driver_update(scancode);
        }

        kthread::yield();
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
        case DOWN_LEFT_BRACKET:     return '[';
        case DOWN_RIGHT_BRACKET:    return ']';
        case DOWN_SPACE:            return ' ';
        case DOWN_SEMICOLON:        return ';';
        case DOWN_SINGLE_QUOTE:     return '\'';
        case DOWN_BACKSLASH:        return '\\';
        case DOWN_COMMA:            return ',';
        case DOWN_PERIOD:           return '.';
        case DOWN_FORWARDSLASH:     return '/';
    }
}






hwdi_PS2Keyboard::hwdi_PS2Keyboard()
:   hwDriverInterface("PS2 Keyboard")
{
    this->irqno   = 1;
    this->handler = kdriver::ps2_kb::irq_handler;
    this->entry   = kdriver::ps2_kb::driver_main;
}


void
hwdi_PS2Keyboard::loadIrqHandler()
{
    // asm volatile ("cli");
    kernel::registerIRQ(this->irqno, this->handler);
    PIC::unmask(this->irqno);
    // asm volatile ("sti");
}



