#pragma once

#include <driver/interface.hpp>
#include <static_vector.hpp>


namespace kernel
{
    extern idk::static_vector<KernelModule, 64> modules;
    // void loadModule( void *addr, size_t size, const char *name=nullptr );
    void loadModules( void *modulebase );
    void initModules();
}

