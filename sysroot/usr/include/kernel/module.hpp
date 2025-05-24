#pragma once

#include <driver/interface.hpp>
#include <static_vector.hpp>


namespace knl
{
    extern idk::static_vector<ModuleInterface*, 64> modules;

    void loadModules( void *tar );
    void initModules();
    ModuleInterface *findModule( uint64_t modtype, uint64_t basetype );
}

