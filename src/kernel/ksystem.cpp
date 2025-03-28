#include "boot/boot.hpp"

#include <libc.h>
#include <libc++>

#include <kdef.h>
#include <kfile.h>
#include <kmalloc.h>
#include <kernel/bitmanip.hpp>
#include <string.h>

// #include "driver/serial.hpp"
#include "log/log.hpp"

#include "ksystem.hpp"
#include "kfs/kfs.hpp"
#include "memory/pmm.hpp"
#include "memory/vmm.hpp"


using namespace idk;

uint64_t kernel::uptime_ms = 0;


KSystem::KSystem( const Krequests &reqs )
:   m_reqs(reqs)
{
    syslog log("KSystem::KSystem");

    cpu0.init();
    
    _load_mmaps();
    kmalloc_init(m_mmaps[0]);
    PMM::init(m_mmaps[1], reqs.hhdm);
    VMM::init();

    libc_init();
    std::detail::libcpp_init();

    tty0 = new kTTY(25*80);

    _load_modules();
    KFS::init((uintptr_t)this);
}


uint64_t
KSystem::getHHDM()
{
    return m_reqs.hhdm;
}


limine_file*
KSystem::getModule( const char *label )
{
    for (auto *file: m_modules)
    {
        if (strcmp(file->string, label) == 0)
        {
            return file;
        }
    }

    return nullptr;
}



#include "memory/pmm.hpp"
#include "memory/vmm.hpp"

int
KSystem::execute( void *address, size_t size, int argc, char **argv )
{
    syslog log("KSystem::execute");

    static uintptr_t entry = 0xBEBE0000;
    static uintptr_t page = 0;

    if (page == 0)
    {
        page = (uintptr_t)kmalloc(8*PMM::PAGE_SIZE);
        page = idk::align_up(page, PMM::PAGE_SIZE);
        VMM::mapRange(page-PMM::hhdm, entry, 7*PMM::PAGE_SIZE);
    }

    log("entry: 0x%lx", entry);
    memset((void*)entry, 0, 6*PMM::PAGE_SIZE);
    memcpy((void*)entry, address, size);

    using YOLO = int (*)(int, char**);
    int result = ((YOLO)entry)(argc, argv);

    log("result: %d", result);

    // VMM::unmapPage(0xBEBE0000);
    // kfree((void*)page);

    return result;
}




const char *mmap_str( uint32_t type )
{
    switch (type)
    {
        default: return "UNKNOWN_TYPE";
        case LIMINE_MEMMAP_USABLE:  return "LIMINE_MEMMAP_USABLE";
        case LIMINE_MEMMAP_RESERVED:    return "LIMINE_MEMMAP_RESERVED";
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:    return "LIMINE_MEMMAP_ACPI_RECLAIMABLE";
        case LIMINE_MEMMAP_ACPI_NVS:    return "LIMINE_MEMMAP_ACPI_NVS";
        case LIMINE_MEMMAP_BAD_MEMORY:  return "LIMINE_MEMMAP_BAD_MEMORY";
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:  return "LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE";
        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:  return "LIMINE_MEMMAP_EXECUTABLE_AND_MODULES";
        case LIMINE_MEMMAP_FRAMEBUFFER: return "LIMINE_MEMMAP_FRAMEBUFFER";
    }
}



void
KSystem::_load_mmaps()
{
    syslog log("KSystem::_load_mmaps");

    auto *res = m_reqs.mmaps;
    auto hhdm = getHHDM();

    m_mmaps = inplace_vector<MemoryMap>(m_mmap_buf, res->entry_count);

    // Load usable memmaps
    for (uint64_t i=0; i<res->entry_count; i++)
    {
        auto *e = res->entries[i];

        if (e->type == LIMINE_MEMMAP_USABLE)
        {
            m_mmaps.push_back(
                MemoryMap(e->base, e->length, hhdm)
            );
        }
    }

    // Sort memmaps
    for (size_t i=0; i<m_mmaps.size(); i++)
    {
        for (size_t j=i+1; j<m_mmaps.size(); j++)
        {
            if (m_mmaps[i].len < m_mmaps[j].len)
            {
                auto tmp = m_mmaps[i];
                m_mmaps[i] = m_mmaps[j];
                m_mmaps[j] = tmp;
            }
        }
    }
}



void
KSystem::lsmem()
{
    syslog log("KSystem::lsmem");

    auto *res = m_reqs.mmaps;
    auto hhdm = getHHDM();

    // Load usable memmaps
    for (uint64_t i=0; i<res->entry_count; i++)
    {
        auto *e = res->entries[i];

        log("%s", mmap_str(e->type));
        log("phys:\t\t0x%lx", e->base);
        log("base:\t\t0x%lx", hhdm + e->base);
        log("end: \t\t0x%lx", hhdm + e->base + e->length);
        log("size:\t\t%luB",  e->length);
        log("     \t\t%luKB", e->length/idk::KILO);
        log("     \t\t%luMB", e->length/idk::MEGA);
    }
}





void
KSystem::_load_modules()
{
    syslog log("KSystem::_load_modules");

    m_modules = inplace_vector<limine_file*>(new limine_file*[64], 64);
    m_execs   = inplace_vector<ExecHeader>(new ExecHeader[32], 32);
    m_fonts   = inplace_vector<FontBuffer>(new FontBuffer[32], 32);

    auto *res = m_reqs.modules;

    for (uint64_t i=0; i<res->module_count; i++)
    {
        auto *file = res->modules[i];
        log("file: \"%s\"", file->string);
        log("path: \"%s\"", file->path);

        size_t len = strlen(file->string);
    
        if (len == 0)
        {
            continue;
        }

        if (strcmp(file->string+len-4, "exec") == 0)
        {
            m_execs.push_back({file->address, file->size});
        }

        else if (strncmp(file->string, "font", 4) == 0)
        {
            auto *header = (ck_BMP_header*)file->address;
            m_fonts.push_back(FontBuffer(file->string, header));
        }

        m_modules.push_back(file);
    }

    log("modules: %d", m_modules.size());
    log("execs:   %d", m_execs.size());
    log("fonts:   %d", m_fonts.size());

}

