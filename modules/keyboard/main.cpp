#include <driver/interface.hpp>
#include <sym/sym.hpp>

#include <arch/io.hpp>
#include <kernel/log.hpp>
#include <kernel/event.hpp>
#include <sys/interrupt.hpp>
#include <kernel/ringbuffer.hpp>
#include <kernel/kscancode.h>
#include <kmemxx.hpp>
#include <algorithm>
#include <ctype.h>


#define STATE_NORMAL 0
#define STATE_PREFIX 1

static knl::RingBuffer<uint8_t, 256> rawstream;
static vfsNode *kbfile;
static char shift_table[256];


static void create_shift_table()
{
    kmemset<uint8_t>(shift_table, '\0', 256);

    for (int i=0; i<256; i++)
    {
        shift_table[i] = char(i);
    }

    char mod_0_9[] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '('};
    kmemcpy<uint8_t>(&(shift_table['0']), mod_0_9, sizeof(mod_0_9));

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
        uvfs::write(kbfile, &event, 0, sizeof(event));

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

    uvfs::write(kbfile, &event, 0, sizeof(event));
}



static void kbdev_irq( intframe_t* )
{
    uint8_t code = IO::inb(0x60);

    if (!rawstream.full())
    {
        rawstream.push_back(code);
    }
}


static void kbev_open()
{
    create_shift_table();
    kbfile = usrknl::popen("/dev/kbevent", sizeof(knl::KbEvent));
    rawstream.clear();
}

static void kbev_close()
{
    // kbfile = uvfs::close("/dev/kbevent", sizeof(knl::KbEvent));
}

static void kbdev_main( void* )
{
    while (true)
    {
        if (!rawstream.empty())
        {   
            uint8_t scancode = rawstream.pop_front();
            kbdev_update(scancode);
        }
        // kthread::yield();
    }
}



static CharDevInterface kbdev;

extern "C"
ModuleInterface *init( ksym::ksym_t *sym )
{
    ksym::loadsym(sym);

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

