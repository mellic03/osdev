// #pragma once

// #include <stddef.h>
// #include <stdint.h>
// #include <cringe/vec.hpp>


// namespace kvideo
// {
//     struct gfxContainer
//     {
//         ivec2 tl, sp;
//         ivec4 margin = ivec4(15);
    
//         int xmin() const { return tl.x        + margin[0]; }
//         int xmax() const { return tl.x + sp.x - margin[1]; }
//         int ymin() const { return tl.y        + margin[2]; }
//         int ymax() const { return tl.y + sp.y - margin[3]; }
//     };


//     // extern ivec2  CSR;
//     // extern int    W;
//     // extern int    H;
//     // extern int    pitch;
//     // extern size_t BPP;
//     // extern size_t nbytes;

//     // extern uint8_t *frontbuffer;
//     // extern uint8_t *backbuffer;

//     // void setCursor( const ivec2& );
//     // const ivec2 &getCursor();
//     void initFrontbuffer( uintptr_t fb_res );
//     void initBackbuffer( uintptr_t fb_res );
//     void fill( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
//     void swapBuffers();
// }



