#pragma once
#include "../gx/gx.hpp"
#include "../gx/gx_assign.hpp"
#include <kernel/vfs2.hpp>
#include <kmath/vec.hpp>
#include <kfs/bmp.hpp>

#include <string.h>
#include <kmalloc.h>
#include <algorithm>
#include <type_traits>


namespace sde
{
    template <typename dtype=uint8_t>
    uint32_t TextureLoad( const BMP_File& );

    template <typename dtype=uint8_t>
    uint32_t TextureLoad( const char *path, int *w=nullptr, int *h=nullptr );

    namespace detail
    {
        template <size_t N, typename dtype>
        void process_texture( void *dstbuf, void *srcbuf, int w, int h );
    }
}



template <typename dtype>
uint32_t
sde::TextureLoad( const BMP_File &bmp )
{
    int w = bmp.w;
    int h = bmp.h;
    void *data = kmalloc(bmp.nbytes);

    switch (bmp.bpp)
    {
        default: break;
        case 8:  detail::process_texture<1, dtype>(data, bmp.data, w, h); break;
        case 16: detail::process_texture<2, dtype>(data, bmp.data, w, h); break;
        case 24: detail::process_texture<3, dtype>(data, bmp.data, w, h); break;
        case 32: detail::process_texture<4, dtype>(data, bmp.data, w, h); break;
    }

    size_t   stride = bmp.bpp/8;
    uint32_t format = 0;

    if constexpr (std::is_same_v<dtype, uint8_t>)
        format = GX_UNSIGNED_BYTE + stride;

    if constexpr (std::is_same_v<dtype, float>)
        format = GX_FLOAT + stride;

    return gxCreateTexture(format, data, w, h);
};


template <typename dtype>
uint32_t
sde::TextureLoad( const char *filepath, int *w, int *h )
{
    auto *fh = vfs2::open<vfs2RawFile>(filepath);

    if (!fh)
    {
        return 0;
    }

    else
    {
        BMP_File bmp(fh->sof);
        if (w) *w = bmp.w;
        if (h) *h = bmp.h;
        return TextureLoad<dtype>(bmp);
    }
}





template <size_t N, typename dtype>
void
sde::detail::process_texture( void *dstbuf, void *srcbuf, int w, int h )
{
    auto *dst = (vec<N, dtype>*)dstbuf;
    auto *src = (uint8_t*)srcbuf;

    // Conversion from bgr --> rgb
    for (int i=0; i<w*h; i++)
    {
        for (size_t j=0; j<N; j++)
        {
            dst[i][j] = static_cast<dtype>(src[N*i + j]);
        }

        if constexpr (std::is_floating_point_v<dtype>)
        {
            dst[i] /= 255.0f;
        }
    }

    // Flip vertically
    for (int i=0; i<h/2; i++)
    {
        for (int j=0; j<w; j++)
        {
            std::swap(dst[w*(h-1-i) + j], dst[w*i + j]);
        }
    }
}


