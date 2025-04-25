#include "gx.hpp"
#include <kmalloc.h>
#include <vector>


static std::vector<void*> gx_buffers;


uint32_t
gxCreateBuffer( size_t nbytes )
{
    gx_buffers.push_back(kmalloc(nbytes));
    return gx_buffers.size() - 1;
}


void
gxDeleteBuffer( uint32_t idx )
{
    kfree(gx_buffers[idx]);
    gx_buffers[idx] = nullptr;
}


void*
gxGetBuffer( uint32_t idx )
{
    return gx_buffers[idx];
}

