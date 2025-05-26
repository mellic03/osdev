// #include <driver/interface.hpp>
// #include <sym/sym.hpp>

// #include <kernel/event.hpp>
// #include <kernel/ringbuffer.hpp>
// #include <kernel/tty.hpp>

// #include <kernel/kscancode.h>
// #include <kmemxx.hpp>
// #include <cringe/bmp.hpp>
// #include <ctype.h>


// static CharDevInterface *kbdev;
// static knlTTY<bool> sdeTTY;


// static void kterm_keyInput()
// {
//     KeyEvent buf[32];

//     size_t nbytes = kbdev->read(buf, sizeof(buf));
//     size_t count  = nbytes / sizeof(KeyEvent);

//     for (size_t i=0; i<count; i++)
//     {
//         sdeTTY.putch(buf[i].key);
//     }
// }


// static void kterm_main( void* )
// {
//     kbdev  = (CharDevInterface*)usrknl::findModule(ModuleType_Device, DeviceType_Keyboard);
//     sdeTTY = knlTTY();

//     while (true)
//     {
//         kterm_keyInput();
    
//         ivec2 csr = kvideo::renderString(&(sdeTTY.buf[0]), ivec2(100, 100));
//         kvideo::renderString("_", csr);
//         kvideo::swapBuffers();
//         kthread::yield();
//     }
// }



// extern "C"
// ModuleInterface *init( ksym::ksym_t *sym )
// {
//     ksym::loadsym(sym);

//     auto *kterm = (DaemonInterface*)std::malloc(sizeof(DaemonInterface));
//     *kterm = {
//         .modtype  = ModuleType_Daemon,
//         .basetype = DaemonType_System,
//         .main     = kterm_main,
//         .start    = nullptr,
//         .stop     = nullptr,
//         .reload   = nullptr,
//         .status   = nullptr,
//     };

//     kmemset<char>(kterm->signature, '\0', sizeof(kterm->signature));
//     kmemcpy<char>(kterm->signature, "kterm", 5);

//     return (ModuleInterface*)kterm;
// }
