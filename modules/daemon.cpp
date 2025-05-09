#include <driver/interface.hpp>
#include <sym/sym.hpp>
#include <kmemxx.hpp>

static void sde_main( void* )
{
    while (true)
    {
        // std::printf("[sde_main]\n");
        kthread::sleep(50);
    }
}




static DaemonInterface daemon;

extern "C"
ModuleInterface *init( ksym::ksym_t *sym )
{
    ksym::loadsym(sym);
    
    kmemset(&daemon, 0, sizeof(daemon));
    daemon = {
        .modtype  = ModuleType_Daemon,
        .basetype = DaemonType_System,
        .subtype  = DaemonType_System,
        .main     = sde_main,
    };

    kmemset<char>(daemon.signature, '\0', sizeof(daemon.signature));
    kmemcpy<char>(daemon.signature, "shitde", 6);

    return (ModuleInterface*)(&daemon);
}

