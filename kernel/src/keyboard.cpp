#include <driver/interface.hpp>

#include <arch/io.hpp>
#include <sys/process.hpp>

#include <kernel/kvideo.hpp>
#include <kernel/log.hpp>
#include <kernel/event.hpp>
#include <kernel/kscancode.h>
#include <kernel/ringbuffer.hpp>

#include <kmemxx.hpp>
#include <algorithm>
#include <ctype.h>


#define STATE_NORMAL 0
#define STATE_PREFIX 1

static knl::RingBuffer<uint8_t, 256> rawstream;
static knl::RingBuffer<knl::KbEvent, 32> keystream;
static char shift_table[256];


bool kbDevGetEvent( knl::KbEvent *dst )
{
    if (keystream.empty())
        return false;
    *dst = keystream.pop_front();
    return true;
}



static void create_shift_table()
{
    kmemset<uint8_t>(shift_table, '\0', 256);

    for (int i=0; i<256; i++)
        shift_table[i] = char(i);

    char mod_0_9[] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '('};
    kmemcpy<uint8_t>(&(shift_table['0']), mod_0_9, sizeof(mod_0_9));

    for (int i=0; i<='z'-'a'; i++)
        shift_table['a'+i] = toupper('a'+i);

    shift_table['/'] = '?';
    shift_table['['] = '{';
    shift_table[']'] = '}';
    shift_table[';'] = ':';
    shift_table['\''] = '\"';
    shift_table[','] = '<';
    shift_table['.'] = '>';

}



static void kbdev_update( uint8_t scancode )
{
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
 
        knl::KbEvent event = {
            .mask = mask,
            .key  = 0
        };
    
        if (!keystream.full())
            keystream.push_back(event);

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

    knl::KbEvent event = {
        .mask = mask,
        .key  = key
    };

    if (!keystream.full())
        keystream.push_back(event);
}



static void kbdev_irq( intframe_t* )
{
    uint8_t code = IO::in8(0x60);

    if (!rawstream.full())
    {
        rawstream.push_back(code);
    }
}


static void kbev_open()
{
    create_shift_table();
    rawstream.clear();
    keystream.clear();
}


static void kbev_close()
{

}


static void kbdev_main( void* )
{
    while (true)
    {
        // syslog::println("[kbdev_main]");
        while (!rawstream.empty())
        {   
            uint8_t scancode = rawstream.pop_front();
            kbdev_update(scancode);
        }
        knl::threadYield();
    }
}



static CharDevInterface kbdev;

ModuleInterface *kbdev_init( void* )
{
    // auto *kbdev = (CharDevInterface*)std::malloc(sizeof(CharDevInterface));

    kbdev.modtype  = ModuleType_Device;
    kbdev.basetype = DeviceType_Keyboard;
    kbdev.main     = kbdev_main;

    kbdev.open     = kbev_open;
    kbdev.close    = kbev_close;
    kbdev.read     = nullptr;
    kbdev.write    = nullptr;
    kbdev.irqno    = IrqNo_Keyboard;
    kbdev.irqfn    = kbdev_irq;

    kmemset<char>(kbdev.signature, '\0', sizeof(kbdev.signature));
    kmemcpy<char>(kbdev.signature, "kboard", 6);

    return (ModuleInterface*)&kbdev;
}

