#include <kernel/module.hpp>
#include <kernel/log.hpp>
#include <sym/sym_type.hpp>
#include <filesystem/ustar.hpp>
#include <kernel/kstring.h>
#include <kthread.hpp>
#include <kmalloc.h>
#include <kpanic.h>

#include <cpu/cpu.hpp>
#include <driver/pic.hpp>


idk::static_vector<KernelModule, 64> kernel::modules;

template <typename ret_type, typename... Args>
static ret_type fncall( void *addr, Args... args )
{
    using fn_type = ret_type (*)(Args...);
    return ((fn_type)addr)(args...);
}


static void load_module( void *base, size_t size, const char *name )
{
    syslog log("kernel::loadModule \"%s\"", name);

    // void *addr = base;
    void *addr = kmalloc(size);
    memcpy(addr, base, size);
    auto *mod = fncall<ModuleInterface*>(addr, ksym::getsym());

    log("addr:     0x%lx", addr);
    log("size:     %lu B", size);
    log("modtype:  %s",  ModuleTypeStr(mod->modtype));
    log("basetype: %s",  ModuleBaseTypeStr(mod->modtype, mod->basetype));
    log("subtype:  %s",  ModuleSubTypeStr(mod->modtype, mod->subtype));
    log("name:     %s",  mod->signature);
    
    kernel::modules.push_back({addr, mod});
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
    for (auto &[addr, mod]: kernel::modules)
    {
        if (mod->modtype == ModuleType_Device)
            init_device((DeviceInterface*)mod);
        if (mod->main)
            kthread::create(mod->signature, mod->main, nullptr);

    }
}

