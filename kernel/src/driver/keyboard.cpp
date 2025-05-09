// #ifndef __is_kernel
//     #define __is_kernel
// #endif

// #include <driver/keyboard.hpp>
// #include <driver/pic.hpp>

// #include <kernel/ioport.hpp>
// #include <kernel/log.hpp>
// #include <kernel/kscancode.h>
// #include <filesystem/vfs2.hpp>
// #include <filesystem/ramfs.hpp>

// #include <kthread.hpp>
// #include <kassert.h>
// #include <kpanic.h>
// #include <ctype.h>


// static void irq_handler( intframe_t* );
// static void driver_main( void* );



// hwPS2Keyboard::hwPS2Keyboard()
// :   hwDriverInterface("PS2 Keyboard")
// {
//     this->irqno   = 1;
//     this->handler = irq_handler;
//     this->entry   = driver_main;
// }


// void
// hwPS2Keyboard::loadIrqHandler()
// {
//     CPU::installIRQ(this->irqno, this->handler);
//     PIC::unmask(this->irqno);
// }





// #define STATE_NORMAL 0
// #define STATE_PREFIX 1

// static vfs2CharDevice *rawfh = nullptr;
// static vfs2CharDevice *keyfh = nullptr;


// static void irq_handler( intframe_t* )
// {
//     uint8_t code = IO::inb(0x60);

//     if (rawfh)
//     {
//         syslog::printf("[keyboard] YEEE\n");
//         rawfh->write(&code, 1);
//     }
// }




// static char shift_table[256];

// static void create_shift_table()
// {
//     memset(shift_table, '\0', 256);

//     for (int i=0; i<256; i++)
//     {
//         shift_table[i] = char(i);
//     }

//     char mod_0_9[] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '('};
//     memcpy(&(shift_table['0']), mod_0_9, sizeof(mod_0_9));

//     for (int i=0; i<='z'-'a'; i++)
//     {
//         shift_table['a'+i] = toupper('a'+i);
//     }

//     shift_table['/'] = '?';
//     shift_table['['] = '{';
//     shift_table[']'] = '}';
//     shift_table[';'] = ':';
//     shift_table['\''] = '\"';
//     shift_table[','] = '<';
//     shift_table['.'] = '>';

// }



// static void
// driver_update( uint8_t scancode )
// {
//     using namespace kdriver::ps2_kb;
//     static uint8_t state = STATE_NORMAL;
//     static uint8_t mask = 0;

//     uint8_t key  = scode_getchar(scancode);

//     if (scancode == 0xE0)
//     {
//         state = STATE_PREFIX;
//         return;
//     }

//     else if (state == STATE_PREFIX)
//     {
//         switch (scancode)
//         {
//             default: break;

//             case 0x48:  mask |=  KeyEvent_U; break;
//             case 0xC8:  mask &= ~KeyEvent_U; break;

//             case 0x4B:  mask |=  KeyEvent_L; break;
//             case 0xCB:  mask &= ~KeyEvent_L; break;

//             case 0x4D:  mask |=  KeyEvent_R; break;
//             case 0xCD:  mask &= ~KeyEvent_R; break;

//             case 0x50:  mask |=  KeyEvent_D; break;
//             case 0xD0:  mask &= ~KeyEvent_D; break;
//         }
 
//         KeyEvent event = {
//             .mask = mask,
//             .key  = 0
//         };

//         keyfh->write(&event, sizeof(KeyEvent));

//         state = STATE_NORMAL;
//         return;
//     }

//     if (scancode >= UP_ESC) mask |=  KeyEvent_UP;
//     else                    mask &= ~KeyEvent_UP;

//     switch (scancode)
//     {
//         default: break;

//         case DOWN_LSHIFT:   mask |=  KeyEvent_SHIFT; break;
//         case UP_LSHIFT:     mask &= ~KeyEvent_SHIFT; break;
    
//         case DOWN_LCTRL:    mask |=  KeyEvent_CTRL;  break;
//         case UP_LCTRL:      mask &= ~KeyEvent_CTRL;  break;
    
//         case DOWN_LALT:     mask |=  KeyEvent_ALT;   break;
//         case UP_LALT:       mask &= ~KeyEvent_ALT;   break;
//     }

//     if (mask & KeyEvent_SHIFT)
//     {
//         key = isalpha(key) ? toupper(key) : shift_table[key];
//     }

//     KeyEvent event = {
//         .mask = mask,
//         .key  = key
//     };

//     keyfh->write(&event, sizeof(KeyEvent));
// }


// static void driver_main( void* )
// {
//     create_shift_table();

//     rawfh = vfs2::find<vfs2CharDevice>("/dev/kb0/raw");
//     keyfh = vfs2::find<vfs2CharDevice>("/dev/kb0/event");

//     // rfsEntry *rawfh2 = rfs::open("/dev/kb0/raw");
//     // rfsEntry *keyfh2 = rfs::open("/dev/kb0/key");

//     kassert("Could not find rawfh", rawfh != nullptr);
//     kassert("Could not find keyfh", keyfh != nullptr);

//     uint8_t scancode;

//     while (true)
//     {
//         if (rawfh->read(&scancode, 1))
//         {
//             driver_update(scancode);
//         }

//         kthread::yield();
//     }

// }
