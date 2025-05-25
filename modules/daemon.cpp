#include <driver/interface.hpp>
#include <sym/sym.hpp>
#include <kernel/input.hpp>
#include <kernel/kscancode.h>
#include <kmemxx.hpp>
#include <cringe/bmp.hpp>
#include <ctype.h>


static CharDevInterface *kbdev;
static char kbtermbuf[256];
static char *kbtermend;
static char *kbtermtop;


static CharDevInterface *msdev;
static kinput::MsData msdata;
static char mousetext[16][16];
static ivec2 mousepos[16];
static int mousecount;

static void reeeee()
{
    kmemcpy<uint8_t>(mousetext[mousecount], "BitchAss\0", sizeof("BitchAss\0"));
    mousepos[mousecount] = msdata.pos();
    mousecount = (mousecount+1) % 16;
}



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
        else if (isalnum(key) || key == ' ')
            *(kbtermtop++) = key;
        if (kbtermtop >= kbtermend)
            kbtermtop = kbtermbuf;
    }
}



static void sde_main( void* )
{
    BMP_File bmp(std::fopen("usr/share/img/cursor.bmp"));

    kbdev = (CharDevInterface*)knl::findModule(ModuleType_Device, DeviceType_Keyboard);
    kmemset<uint8_t>(kbtermbuf, '\0', sizeof(kbtermbuf));
    kbtermend = kbtermbuf + sizeof(kbtermbuf);
    kbtermtop = kbtermbuf;
    // std::printf("kbdev: %s\n", kbdev->signature);

    msdev = (CharDevInterface*)knl::findModule(ModuleType_Device, DeviceType_Mouse);
    kmemset<uint8_t>(mousetext, '\0', sizeof(mousetext));
    kmemset<uint8_t>(mousepos, 0, sizeof(mousepos));
    mousecount = 0;

    kinput::MsCallbacks callbacks;
    kinput::readMsCallbacks(callbacks);
    callbacks.onUp[0].l = reeeee;
    kinput::writeMsCallbacks(callbacks);

    uint64_t prev_time;
    uint64_t curr_time;
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

        if (ready == false)
        {
            *kvideo::CSR = ivec2(50, 50);
            kvideo::cursorString(kbtermbuf);

            msdev->read(&msdata, sizeof(msdata));
            // kvideo::fillColor(200, 50, 50, 255);
            // kvideo::rect(msdata.x, msdata.y, 100, 100);

            for (int i=0; i<mousecount; i++)
                kvideo::renderString(mousetext[i], mousepos[i]);

            kvideo::blit(
                ivec2(msdata.x, msdata.y),
                (uint8_t*)(bmp.data),
                bmp.w, bmp.h,
                ivec2(0, 0), ivec2(bmp.w, bmp.h)
            );
        
            ready = true;
        }
    
        if (ready && accum > 16)
        {
            ready = false;
            accum = (accum-16) % 16;

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
