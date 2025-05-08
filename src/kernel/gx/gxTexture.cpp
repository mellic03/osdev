#include "gx.hpp"
#include "gxTexture.hpp"
#include "command/macros.hpp"
#include <kmalloc.h>
#include <mutex>
#include <vector>


static std::vector<gxTexture> gx_textures;
static std::vector<uint32_t>  gx_freelist;
static std::mutex             gx_mutex;

__attribute__((constructor))
static void gx_texture_init( void )
{
    static constexpr int max_textures = 256;

    gx_textures.reserve(max_textures);
    gx_freelist.reserve(max_textures);
}

static bool isValidID( uint32_t texid )
{
    return (texid < gx_textures.size()) && (gx_textures[texid].status == gxResourceStatus_Active);
}

// static int findIndex()
// {
//     for (size_t i=0; i<gx_textures.size(); i++)
//     {

        
//     }
// }


#include <kernel/log.hpp>

uint32_t
gxCreateTextureExplicit( uint32_t flags, uint32_t format, void *data, int w, int h )
{
    gx_mutex.lock();

    uint32_t idx = 0;

    if (gx_freelist.empty())
    {
        gx_textures.push_back(gxTexture());
        idx = gx_textures.size() - 1;
    }

    else
    {
        idx = gx_freelist.back();
              gx_freelist.pop_back();
    }

    size_t stride = gxGetFormatStride(format);
    size_t nbytes = w*h * stride;
    void *pixels = nullptr;

    if (flags & gxResourceFlag_ExplicitAlloc)
    {
        pixels = data;
    }

    else
    {
        pixels = kmalloc(nbytes);

        if (data)
        {
            memcpy(pixels, data, nbytes);
        }
    }

    gx_textures[idx] = {
        .status = gxResourceStatus_Active,
        .flags  = (gxResourceFlag_)flags,
        .format = format,
        .stride = stride,
        .data   = pixels,
        .w      = w,
        .h      = h
    };

    gx_mutex.unlock();

    return idx;
}


uint32_t
gxCreateTexture( uint32_t format, void *data, int w, int h )
{
    return gxCreateTextureExplicit(gxResourceFlag_None, format, data, w, h);
}


void
gxDeleteTexture( uint32_t idx )
{
    if (!isValidID(idx))
    {
        syslog::kprintf("[gxGetTexture] invalid texture ID: %u\n", idx);
        return;
    }

    auto &tex = gx_textures[idx];

    if ((tex.flags & gxResourceFlag_ExplicitFree) == false)
    {
        kfree(tex.data);
    }

    tex.status = gxResourceStatus_Deleted;
    tex.data   = nullptr;

    gx_freelist.push_back(idx);
}



gxTexture*
gxGetTexture( uint32_t idx )
{
    if (!isValidID(idx))
    {
        syslog::kprintf("[gxGetTexture] invalid texture ID: %u\n", idx);
        return nullptr;
    }

    return &gx_textures[idx];
}

