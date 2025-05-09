#include <driver/interface.hpp>
#include <sym/sym.hpp>
#include <kmemxx.hpp>


static void sde_main( void* )
{
    auto *iface = kernel::findModule(ModuleType_Device, DeviceType_Mouse);
    auto *msdev = (CharDevInterface*)iface;

    std::printf("[sde_main] iface:       0x%lx\n", iface);
    std::printf("[sde_main] iface->sig:  %s\n",    iface->signature);
    std::printf("[sde_main] msdev->read: 0x%lx\n", msdev->read);
    // std::printf("[sde_main] mousedev: 0x%lx\n", msdev);

    int xpos = 200;
    int ypos = 200;


    while (true)
    {
        int xy[2];

        if (msdev->read(xy, sizeof(xy)) == sizeof(xy))
        {
            xpos = xy[0];
            ypos = xy[1];
        
        }

        // std::printf("[sde_main] xpos=%d\n", xpos);
        kvideo::rect(xpos, ypos, 400, 400);

        kvideo::swapBuffers();
        kthread::yield();
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

