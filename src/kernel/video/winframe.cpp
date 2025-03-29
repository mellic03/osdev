// #include "buf_wrapper.hpp"
// #include "font.hpp"
// #include "winframe.hpp"
// #include "video.hpp"

// // ---------------------------------------------------------------------------------------------
// idk::WinFrameBase::WinFrameBase( const vec2 &tl, const vec2 &sp )
// :   m_tl(tl),
//     span(sp),
//     m_bg(255),
//     m_depth(0.9999f),
//     m_parent(nullptr),
//     m_children(cbuf, 8)
// {

// }


// void
// idk::WinFrameBase::giveChild( WinFrameBase *child )
// {
//     child->m_parent = this;
//     m_children.push_back(child);
// }
// // ---------------------------------------------------------------------------------------------








// // ---------------------------------------------------------------------------------------------
// #include <kernel/ioport.hpp>

// void
// idk::WinFrameSolid::render( idk::Video &video )
// {
//     ivec2 tl = ivec2(m_tl);
//     ivec2 sp = ivec2(span);
//     ivec2 bd = ivec2(0); // m_style.border;

//     video.rect(tl, sp, m_bg, m_depth);
// }
// // ---------------------------------------------------------------------------------------------




// // ---------------------------------------------------------------------------------------------
// idk::WinFrameTTY::WinFrameTTY( idk::Terminal *tty, idk::FontBuffer *font )
// :   WinFrameBase({0, 0}, {0, 0}),
//     m_tty(tty),
//     m_font(font)
// {
    
// }


// void
// idk::WinFrameTTY::render( idk::Video &video )
// {
//     // auto *font = m_font;
//     // const char *data = m_tty->data();
//     // int csr = 0;

//     // while (csr < m_tty->CSR)
//     // {
//     //     int  row = csr / m_tty->W;
//     //     int  col = csr % m_tty->W;
//     //     char val = data[csr];
//     //     csr += 1; 

//     //     auto *bd = font->getGlyph(val);

//     //     if (!bd)
//     //     {
//     //         continue;
//     //     }

//     //     ivec2 src  = bd->corner;
//     //     ivec2 span = bd->extents;

//     //     ivec2 dst  = ivec2(
//     //         m_tl.x + span.x*col,
//     //         m_tl.y + span.y*row
//     //     );

//     //     video.blit(dst, src, span, buf_wrapper<uint32_t>(font->W, (*font)[0]), m_depth);
//     // }
// }
// // ---------------------------------------------------------------------------------------------









