#include <kernel/module.hpp>
#include <kernel/log.hpp>
#include <sym/sym_t.hpp>
#include <filesystem/ustar.hpp>
#include <kernel/kstring.h>
#include <kernel/memory/pmm.hpp>
#include <kthread.hpp>
#include <kmalloc.h>
#include <kpanic.h>

#include <cpu/cpu.hpp>
#include "../smp/smp.hpp"
#include <driver/pic.hpp>
#include <idk_fptr.hpp>

#include <arch/apic.hpp>
#include <arch/elf.h>
#include <tuple>

idk::static_vector<ModuleInterface*, 64> knl::modules;

template <typename ret_type, typename... Args>
static ret_type fncall( void *addr, Args... args )
{
    using fn_type = ret_type (*)(Args...);
    return ((fn_type)addr)(args...);
}





static void init_device( DeviceInterface *dev )
{
    if (dev->open)
    {
        dev->open();
    }

    if (dev->irqfn && dev->irqno >= 0)
    {
        CPU::installIRQ(dev->irqno, dev->irqfn);
        IOAPIC::mapIRQ(SMP::this_cpuid(), dev->irqno);
    }
}



static void load_module( uint8_t *tar, size_t size )
{
    syslog log("knl::loadModule");

    auto *data = tar + ustar::DATA_OFFSET;
    auto *base = (uint8_t*)(PMM::alloc() + PMM::hhdm);
    memcpy(base, data, size);

    auto *ehdr  = (Elf64_Ehdr*)base;
    void *entry = (uint8_t*)base + ehdr->e_entry;
    auto *iface = fncall<ModuleInterface*>(entry, ksym::getsym());

    log("base:     0x%lx", base);
    log("entry:    0x%lx", entry);
    log("end:      0x%lx", (uint8_t*)base + std::max(size, PMM::PAGE_SIZE));
    log("modtype:  %s",    ModuleTypeStr(iface->modtype));
    log("basetype: %s",    ModuleBaseTypeStr(iface->modtype, iface->basetype));
    
    iface->baseAddress = base;
    iface->pageSize    = PMM::PAGE_SIZE;
    iface->pageCount   = 1;

    knl::modules.push_back(iface);
}

extern void loadElf64( void *data, size_t size );


void knl::loadModules( void *tar )
{
    syslog log("knl::loadModules");

    if (tar == nullptr)
        kpanic("[knl::loadModules] cannot find modules");
    // ustar::listChilren(tar);

    ustar::forEach(tar, [&log](void *ctar, size_t fsize)
    {
        auto *ext  = seek_ch((const char*)ctar, '.');
        if (*ext && (strcmp(ext, ".elf") == 0))
        {
            log("File \"%s\"", (const char*)ctar + ustar::NAME_OFFSET);
            load_module((uint8_t*)ctar, fsize);
        }
    });

}



void knl::initModules()
{
    for (auto *I: knl::modules)
    {
        if (I->modtype == ModuleType_Device)
            init_device((DeviceInterface*)I);
        if (I->main)
        {
            kthread::create(I->signature, I->main, nullptr);

            // auto *th = kthread::create(I->signature, I->main, nullptr);
            // if (strncmp(I->signature, "mouse", 5) == 0)
            //     th->setPriority(4);
            // else if (strncmp(I->signature, "shitde", 6) == 0)
            //     th->setPriority(5);
            // else
            //     th->setPriority(4);
        }
    }
}


ModuleInterface *knl::findModule( uint64_t mt, uint64_t bt )
{
    for (auto *I: knl::modules)
        if ((I->modtype==mt) && (I->basetype==bt))
            return I;
    return nullptr;
}


