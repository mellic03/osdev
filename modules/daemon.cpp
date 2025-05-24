#include <driver/interface.hpp>
#include <sym/sym.hpp>
#include <kernel/input.hpp>
#include <kmemxx.hpp>


static void sde_main( void* )
{
    // auto *iface = kernel::findModule(ModuleType_Device, DeviceType_Mouse);
    // auto *msdev = (CharDevInterface*)iface;

    // std::printf("[sde_main] iface:       0x%lx\n", iface);
    // std::printf("[sde_main] iface->sig:  %s\n",    iface->signature);
    // std::printf("[sde_main] msdev->read: 0x%lx\n", msdev->read);
    // std::printf("[sde_main] mousedev: 0x%lx\n", msdev);

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
    
        kvideo::fillColor(200, 50, 50, 255);
        kvideo::rect(msdata.x.load(), msdata.y.load(), 100, 100);
        ready = true;
        // kthread::yield();
    
        if (ready && accum >= 16)
        {
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
