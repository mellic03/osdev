#define LIMINE_API_REVISION 3
#include <limine/limine.h>


#include <kdef.h>
#include <kmalloc.h>
#include <kdriver/serial.hpp>
#include <libc/libc_init.h>
#include <libc++/libc++_init>

#include "./ksystem.hpp"
#include "./filesystem/filesystem.hpp"
using namespace idk;


extern kret_t kvirtio_init();


KSystem::KSystem( const KRequests &reqs )
{
    SYSLOG_BEGIN("KSystem::KSystem");

    m_reqs = reqs;
    cpu0.init();
    
    _load_mmaps();

    kmalloc_init((void*)(m_mmaps[0].addr), m_mmaps[0].len);
    kvirtio_init();

    _load_modules();

    kfilesystem_init();
    libc_init();
    libcpp_init();

    SYSLOG_END();
}


uint64_t
KSystem::getHHDM()
{
    return m_reqs.hhdm->offset;
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



int
KSystem::execute( ExecHeader *exec, int argc, char **argv )
{
    void  *exec_addr = (void*)(0xFFFF800100000000);
    memcpy(exec_addr, exec->addr, exec->size);

    using YOLO = int (*)(int, char**);
    int retvalue = ((YOLO)exec_addr)(argc, argv);

    return retvalue;
}






void
KSystem::_load_mmaps()
{
    auto *res = m_reqs.mmaps;
    auto hhdm = m_reqs.hhdm->offset;

    m_mmaps = inplace_vector<MemoryMap>(m_mmap_buf, res->entry_count);

    // Load usable memmaps
    for (uint64_t i=0; i<res->entry_count; i++)
    {
        auto *e = res->entries[i];

        if (e->type == LIMINE_MEMMAP_USABLE)
        {
            m_mmaps.push_back(
                MemoryMap(hhdm+e->base, e->length)
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
KSystem::_load_modules()
{
    m_modules = inplace_vector<limine_file*>(new limine_file*[64], 64);
    m_execs   = inplace_vector<ExecHeader>(new ExecHeader[32], 32);

    SYSLOG_BEGIN("KSystem::_load_modules");
    auto *res = m_reqs.modules;

    for (uint64_t i=0; i<res->module_count; i++)
    {
        auto *file = res->modules[i];
        SYSLOG("file: \"%s\"", file->string);

        size_t len = strlen(file->string);
        if (len >= 4 && strcmp(file->string+len-4, "exec") == 0)
        {
            m_execs.push_back({file->address, file->size});
        }
        
        m_modules.push_back(file);
    }

    SYSLOG("modules: %d\n", m_modules.size());
    SYSLOG("execs:   %d\n", m_execs.size());


    SYSLOG_END();


    // for (size_t i=0; i<res->module_count; i++)
    // {
    //     auto *file = res->modules[i];

    //     if (strncmp(file->string, "exec", 4) == 0)
    //     {
    //         SYSLOG("exec: \"%s\"\n", file->string);
    //         // auto *header = (ck_BMP_header*)file->address;
    //         // m_fonts.push_back(FontBuffer(file->string, header));
    //     }
    // }
}

