#pragma once

#include "../kvideo/kvideo.hpp"
#include "context.hpp"

#include <kdef.h>
#include <kmemxx.h>
#include <kmath/vec.hpp>
#include <kinplace/inplace_vector.hpp>



void sde_main( void* );


namespace sde
{
    WindowContext *createContext( ivec2 corner, ivec2 span );
    void flushContext( WindowContext* );
    void destroyContext( WindowContext* );

    WindowContext *getCurrent();
    void makeCurrent( WindowContext* );

    void blendMode( int mode );

    void hline( int x0, int x1, int y,  const vec4& );
    void vline( int x,  int y0, int y1, const vec4& );
    void rectOutline( ivec2 tl, ivec2 sp, const vec4& );
    void rect( ivec2 tl, ivec2 sp, const vec4& );
    void blit( ivec2 tl0, ivec2 tl1, ivec2 sp, const kframebuffer<vec4>& );

}


