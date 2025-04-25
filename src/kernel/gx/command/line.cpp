#ifndef __is_kernel
    #define __is_kernel
#endif

#include "../gx.hpp"
#include "../command.hpp"
#include "../../kvideo/kvideo.hpp"
#include <kernel/log.hpp>

// static void line_vec4( const gxDrawCmdLine &cmd )
// {
    
// }


// using linefn_type = void (*)( const gxDrawCmdLine& );
// static linefn_type dispatch_table[8];

// __attribute__((constructor))
// static void dispatch_init( void )
// {
//     dispatch_table[GX_RGBA8] = rect_rgba8u;
//     dispatch_table[GX_RGBA32F] = rect_vec4;
// }


void gx_ExecCommand_Line( const gxDrawCmd &cmd )
{
    auto &lcmd = cmd.data.line;
    auto *dst = gxGetTexture(lcmd.dst);
    // auto idx  = cmd.dst->format;

    if (dst->format != GX_RGBA32F)
    {
        return;
    }

    // if (idx >= 32)
    //     return;
    // if (dispatch_table[idx] == nullptr)
    //     return;

    // dispatch_table[idx](cmd);

}


