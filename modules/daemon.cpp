#include <driver/interface.hpp>
#include <sym/sym.hpp>
#include <kernel/input.hpp>
#include <kmemxx.hpp>
#include <cringe/bmp.hpp>


static void sde_main( void* )
{
    BMP_File bmp(std::fopen("usr/share/img/cursor.bmp"));
    std::printf("[sde_main] bmp.bpp: %d", bmp.bpp);

    kinput::MsData msdata;
    msdata.x = 50;
    msdata.y = 50;

    uint64_t prev_time;
    uint64_t curr_time;
    uint64_t accum = 0;

    bool ready = false;

    while (true)
    {
        curr_time = std::clock();
        accum += (curr_time - prev_time);
        prev_time = curr_time;
        // std::printf("[daemon.cpp] clock: %lu\n", std::clock());

        kinput::readMsData(&msdata);

        // if (ready == false)
        {
            // kvideo::fillColor(200, 50, 50, 255);
            // kvideo::rect(msdata.x, msdata.y, 100, 100);
        
            kvideo::blit(
                ivec2(msdata.x, msdata.y),
                (uint8_t*)(bmp.data),
                bmp.w, bmp.h,
                ivec2(0, 0), ivec2(bmp.w, bmp.h)
            );
        
            ready = true;
        }
    
        if (ready && accum >= 16)
        {
            ready = false;
            accum = 0;

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

    // std::printf("[daemon.cpp] daemon: 0x%lx\n", daemon);

    return (ModuleInterface*)daemon;
}
