#include <kernel/interrupt.hpp>
#include <kernel/ioport.hpp>
#include <kernel/kscancode.h>
#include <kmemxx.hpp>
#include <ctype.h>


// int global_var = 42;
// void redirect_got() {
//     void **got_entry = (void **)(global_var@GOTPCREL + (uintptr_t)&redirect_got);
//     *got_entry = (void *)0xdeadbeef;  // Change GOT entry to point elsewhere
// }


#define STATE_NORMAL 0
#define STATE_PREFIX 1

static uint8_t raw_buf[256];
static uint8_t raw_head;
static uint8_t raw_tail;

static KeyEvent key_buf[256];
static uint8_t key_head;
static uint8_t key_tail;


extern "C"
{
    #if UINT32_MAX == UINTPTR_MAX
        #define STACK_CHK_GUARD 0xe2dee396
    #else
        #define STACK_CHK_GUARD 0x595e9fbd94fda766
    #endif
    
    uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
    
    __attribute__((noreturn))
    void __stack_chk_fail(void)
    {
        // kpanic("Stack smashing detected");
        while (true) {  };
    }
}



char shift_table[256];

void create_shift_table()
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



void driver_update( uint8_t scancode )
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
 
        KeyEvent event = {
            .mask = mask,
            .key  = 0
        };

        key_buf[key_tail++] = event;

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

    key_buf[key_tail++] = event;
}



#include <kernel/module.hpp>
#include <sym/sym.hpp>


void driver_main( void* )
{
    create_shift_table();

    raw_head = 0;
    raw_tail  = 0;
    key_head = 0;
    key_tail  = 0;

    uint8_t scancode;

    while (true)
    {
        while (raw_tail > 0)
        {
            scancode = raw_buf[raw_head++];
            raw_tail--;
            driver_update(scancode);
        }
    }

}


size_t driver_read( void *dst, size_t nbytes )
{
    auto  *dstbuf  = (KeyEvent*)dst;
    size_t count = 0;

    while (key_tail > 0 && count < nbytes)
    {
        *(dstbuf++) = key_buf[key_head++];
        key_tail--;
        count++;
    }

    return count;
}

size_t driver_write( const void *, size_t  )
{
    return 0;
}


void irq_handler( intframe_t* )
{
    uint8_t code = IO::inb(0x60);
    raw_buf[raw_tail++] = code;
}



static CharDeviceInterface interface;
static iTableEntry table[3];

extern "C"
iTableEntry *init( void* )
{
    interface = CharDeviceInterface(
        driver_main, nullptr, driver_read, driver_write
    );

    table[0] = { ITABLE_BEGIN, NULL };
    table[1] = { ITABLE_CHAR_DEVICE, &interface };
    table[2] = { ITABLE_END, NULL };

    return table;
}

