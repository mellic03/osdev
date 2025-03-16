#include "./window.hpp"
#include "./video.hpp"




idk::WinFrameBase::WinFrameBase( const vec2 &tl, const vec2 &sp )
:   m_tl(tl),
    m_sp(sp),
    m_bg(255),
    m_depth(0.9999f),
    m_parent(nullptr),
    m_children(8, cbuf)
{

}


void
idk::WinFrameBase::giveChild( WinFrameBase *child )
{
    child->m_parent = this;
    m_children.push_back(child);
}



void
idk::WinFrameSolid::render( idk::Video &video )
{
    ivec2 tl = ivec2(m_tl);
    ivec2 sp = ivec2(m_sp);
    ivec2 bd = ivec2(0); // m_style.border;

    video.rect(tl+bd, sp-bd, m_bg, m_depth);

}





idk::WinFrameTerminal::WinFrameTerminal( idk::Terminal *tty, idk::FontBuffer *font )
:   WinFrameBase({0, 0}, {0, 0}),
    m_tty(tty),
    m_font(font)
{
    
}


void
idk::WinFrameTerminal::render( idk::Video &video )
{
    auto *font = m_font;
    const char *data = m_tty->data();
    int csr = 0;

    while (csr < m_tty->CSR)
    {
        int  row = csr / m_tty->W;
        int  col = csr % m_tty->W;
        char val = data[csr];
        csr += 1; 

        auto *bd = font->getGlyph(val);

        if (!bd)
        {
            continue;
        }

        ivec2 src  = bd->corner;
        ivec2 span = bd->extents;

        ivec2 dst  = ivec2(
            m_tl.x + span.x*col,
            m_tl.y + span.y*row
        );

        video.blit(dst, src, span, buf_wrapper<uint32_t>(font->W, (*font)[0]), m_depth);
    }

}























// idk::VWindow::VWindow( int id, const ivec2 &cnr, const ivec2 &ext, const uvec4 &color )
// :   ID(id),
//     dirty(false),
//     corner(cnr),  extents(ext), bg(color),
//     x(corner.x),  y(corner.y),
//     w(extents.x), h(extents.y),
//     depth(128),
//     term(nullptr),
//     font(nullptr)
// {
//     term    = nullptr;
//     font    = nullptr;
//     corner  = cnr;
//     extents = ext;
//     bg      = color;

//     for (int i=0; i<4; i++)
//     {
//         children[i] = nullptr;
//     }
// }



// void
// idk::VWindow::setpos( const ivec2 &pos )
// {
//     corner = pos;
//     dirty = true;
// }


// void
// idk::VWindow::translate( const ivec2 &delta )
// {
//     if (delta == 0)
//     {
//         return;
//     }

//     corner += delta;
//     dirty = true;
// }


// void
// idk::VWindow::resize( const ivec2 &size )
// {
//     extents = size;
//     dirty = true;
// }





