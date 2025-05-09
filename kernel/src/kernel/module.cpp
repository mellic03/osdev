#include <kernel/module.hpp>
#include <kernel/log.hpp>
#include <sym/sym_t.hpp>
#include <filesystem/ustar.hpp>
#include <kernel/kstring.h>
#include <kthread.hpp>
#include <kmalloc.h>
#include <kpanic.h>

#include <cpu/cpu.hpp>
#include <driver/pic.hpp>


idk::static_vector<ModuleInterface*, 64> kernel::modules;

template <typename ret_type, typename... Args>
static ret_type fncall( void *addr, Args... args )
{
    using fn_type = ret_type (*)(Args...);
    return ((fn_type)addr)(args...);
}


static void load_module( void *base, size_t size, const char *name )
{
    syslog log("kernel::loadModule \"%s\"", name);

    void *addr = kmalloc(size);
    memcpy(addr, base, size);

    ModuleInterface *iface = fncall<ModuleInterface*>(addr, ksym::getsym());
    log("addr:     0x%lx", addr);
    log("size:     %lu B", size);
    log("modtype:  %s",  ModuleTypeStr(iface->modtype));
    log("basetype: %s",  ModuleBaseTypeStr(iface->modtype, iface->basetype));
    log("name:     %s",  iface->signature);
    
    kernel::modules.push_back(iface);
}


void kernel::loadModules( void *modbase )
{
    syslog log("kernel::loadModules");

    if (!modbase)
    {
        kpanic("[kernel::loadModules] cannot find modules");
        return;
    }

    ustar::list(modbase);

    ustar::forEach(modbase, [&log](const char *name, void *base, size_t fsize)
    {
        auto *ext = seek_ch(name, '.');
        if (*ext && (strcmp(ext, ".bin") == 0))
            load_module(base, fsize, name);
    });

}



static void init_device( DeviceInterface *dev )
{
    syslog log("init_device");

    log("%s->read: 0x%lx", dev->signature, dev->read);

    if (dev->open)
    {
        dev->open();
    }

    if (dev->isrfn && dev->isrno != -1)
    {
        CPU::installIRQ(dev->isrno, dev->isrfn);
        PIC::unmask(dev->isrno);
    }
}


void kernel::initModules()
{
    syslog log("initModules");

    for (auto *iface: kernel::modules)
    {
        if (iface->modtype == ModuleType_Device)
            init_device((DeviceInterface*)iface);
        if (iface->main)
            kthread::create(iface->signature, iface->main, nullptr);

    }
}


ModuleInterface *kernel::findModule( uint64_t mt, uint64_t bt )
{
    syslog log("findModule");

    for (size_t i=0; i<kernel::modules.size(); i++)
    {
        auto *I = kernel::modules[i];
        if ((I->modtype==mt) && (I->basetype==bt))
            return I;
    }
    return nullptr;
}


