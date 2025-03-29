// #pragma once

// #include <stddef.h>
// #include <stdint.h>

// #include "font.hpp"
// #include "winframe.hpp"
// #include <kinplace/inplace_vector.hpp>
// #include <kernel/memory.hpp>
// #include <kernel/terminal.hpp>
// #include <kmath/vec.hpp>



// namespace idk
// {
//     template <typename T>
//     struct buf_wrapper;

//     class WinFrameBase;
//     class WinFrameSolid;
//     class WinFrameTTY;

//     class Video;
// }





// // ---------------------------------------------------------------------------------------------
// class idk::WinFrameBase
// {
// private:
//     WinFrameBase *cbuf[8];

// public:
//     ivec2 m_tl;
//     ivec2 span;
//     uvec4 m_bg;
//     float m_depth;

//     WinFrameBase *m_parent;
//     idk::inplace_vector<WinFrameBase*> m_children;

//     WinFrameBase( const vec2 &tl, const vec2 &sp );
//     virtual ~WinFrameBase() = default;
//     virtual void render( idk::Video &video ) = 0;

//     void giveChild( WinFrameBase* );

// };
// // ---------------------------------------------------------------------------------------------





// // ---------------------------------------------------------------------------------------------
// class idk::WinFrameSolid: public idk::WinFrameBase
// {
// private:

// public:
//     using WinFrameBase::WinFrameBase;
//     virtual void render( idk::Video &video ) override;

// };
// // ---------------------------------------------------------------------------------------------





// // ---------------------------------------------------------------------------------------------
// class idk::WinFrameTTY: public idk::WinFrameBase
// {
// private:
//     idk::Terminal   *m_tty;
//     idk::FontBuffer *m_font;

// public:
//     WinFrameTTY( idk::Terminal *tty, idk::FontBuffer *font );
//     virtual void render( idk::Video &video ) override;
// };
// // ---------------------------------------------------------------------------------------------
