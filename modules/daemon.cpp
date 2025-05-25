#include <driver/interface.hpp>
#include <sym/sym.hpp>

#include <functional>
#include <tuple>
#include <kernel/event.hpp>
#include <kernel/ringbuffer.hpp>

#include <kernel/kscancode.h>
#include <kmemxx.hpp>
#include <cringe/bmp.hpp>
#include <ctype.h>


// using MsCallback = void (*)(const kevent::MsEvent&);
// static knl::RingBuffer<MsCallback, 32> mouseCallbacks;
// // using KbCallback = std::function<void(const kevent::KbEvent&)>; // void (*)(void);
// // static knl::RingBuffer<KbCallback, 32> kboardCallbacks;


static CharDevInterface *msdev;
static knl::MsState msdata;

static CharDevInterface *kbdev;
static char kbtermbuf[256];
static char *kbtermend;
static char *kbtermtop;



static void sde_keyInput()
{
    KeyEvent buf[32];

    size_t nbytes = kbdev->read(buf, sizeof(buf));
    size_t count  = nbytes / sizeof(KeyEvent);

    for (size_t i=0; i<count; i++)
    {
        auto &event = buf[i];
        uint8_t key = event.key;

        if (key == '\b')
            *(--kbtermtop) = '\0';
        else if ((' ' <= key && key <= '~') || key == '\n')
            *(kbtermtop++) = key;

        if (kbtermtop >= kbtermend)
            kbtermtop = kbtermbuf;
    }
}



// static void testCallback( const knl::KbEvent &event )
// {
//     uint8_t key = event.key;

//     if (key == '\b')
//         *(--kbtermtop) = '\0';
//     else if ((' ' <= key && key <= '~') || key == '\n')
//         *(kbtermtop++) = key;

//     if (kbtermtop >= kbtermend)
//         kbtermtop = kbtermbuf;
// }

// static void registerCallbacks()
// {
//     // static int id0 = usrknl::listenKbEvent(testCallback);
//     usrknl::listenKbEvent(testCallback);

// }



static void sde_main( void* )
{
    // registerCallbacks();

    BMP_File bmp(std::fopen("usr/share/img/cursor.bmp"));

    kbdev = (CharDevInterface*)usrknl::findModule(ModuleType_Device, DeviceType_Keyboard);
    msdev = (CharDevInterface*)usrknl::findModule(ModuleType_Device, DeviceType_Mouse);

    kmemset<uint8_t>(kbtermbuf, '\0', sizeof(kbtermbuf));
    kbtermend = kbtermbuf + sizeof(kbtermbuf);
    kbtermtop = kbtermbuf;

    uint64_t prev_time = 0;
    uint64_t curr_time = 0;
    uint64_t accum = 0;
    bool ready = false;

    while (true)
    {
        curr_time = std::clock();
        accum += (curr_time - prev_time);
        prev_time = curr_time;
        // std::printf("[daemon.cpp] cpu=%lu, clock=%lu\n", kthread::this_cpuid(), std::clock());
        // std::printf("[daemon.cpp] cpu=%lu\n", kthread::this_cpuid());

        sde_keyInput();
        msdev->read(&msdata, sizeof(msdata));
    
        if (ready == false)
        {
            *kvideo::CSR = ivec2(50, 50);
            kvideo::cursorString(kbtermbuf);

            kvideo::blit(
                ivec2(msdata.x, msdata.y),
                (uint8_t*)(bmp.data),
                bmp.w, bmp.h,
                ivec2(0, 0), ivec2(bmp.w, bmp.h)
            );
        
            ready = true;
        }
    
        if (ready && accum > 16667)
        {
            ready = false;

            accum /= 1000;
            accum = (accum-16) % 16;
            accum *= 1000;

            kvideo::swapBuffers();
            kthread::yield();
        }
    }
}



extern "C"
ModuleInterface *init( ksym::ksym_t *sym )
{
    ksym::loadsym(sym);

    auto *daemon = (DaemonInterface*)std::malloc(sizeof(DaemonInterface));
    *daemon = {
        .modtype  = ModuleType_Daemon,
        .basetype = DaemonType_System,
        .main     = sde_main,
        .start    = nullptr,
        .stop     = nullptr,
        .reload   = nullptr,
        .status   = nullptr
    };

    kmemset<char>(daemon->signature, '\0', sizeof(daemon->signature));
    kmemcpy<char>(daemon->signature, "shitde", 6);

    return (ModuleInterface*)daemon;
}
