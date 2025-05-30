#include <wm/frame.hpp>
#include <wm/render.hpp>


void wm::guiWindow::onMouseMove( guiMouse &ms )
{
    if (ms.dragging)
    {
        m_tl += ms.delta;
    }
}


void wm::guiWindow::draw( guiFramebuffer &dst )
{
    guiRenderRect(dst, u8vec4(255, 50, 50, 255), m_tl, ivec2(m_sp.x, 50));
    guiRenderBox(dst, u8vec4(50, 255, 50, 255), m_tl, m_sp);

    auto *curr = m_child;
    while (curr)
    {
        curr->draw(dst);
        curr = curr->m_next;
    }
};


void wm::guiRoot::onMouseMove( guiMouse & )
{
    // auto *curr = m_child;

    // while (curr)
    // {
    //     // curr->draw(dst);
    //     curr = curr->m_next;
    // }
}


void wm::guiRoot::onClick( guiMouse &ms )
{
    ms.focused = ms.hovered;

    if (ms.focused && (ms.focused != this))
    {
        ms.focused->makeLast();
        ms.focused->onClick(ms);
        ms.dragging = true; 
    }
}




// static void wm_ForAllDFS( wm::guiElement *curr, void (*action)(wm::guiElement*, void*) )
// {
//     auto *child = curr->m_child;
//     while (child)
//     {
//         wm_ForAllDFS(child, action);
//         child = child->m_prev;
//     }

//     action(curr);
// }


void wm::guiRoot::draw( guiFramebuffer &dst )
{
    m_tl = dst.m_tl;
    m_sp = dst.m_sp;

    auto *curr = m_child;
    while (curr)
    {
        curr->draw(dst);
        curr = curr->m_next;
    }
};





// guiTextArea::guiTextArea( const ivec2 &corner, const ivec2 &span, const ivec4 &mgn )
// :   guiContainer(corner, span, mgn)
// {
//     idx     = 0;
//     vscroll = 0;
//     color   = u8vec4(50, 50, 50, 255);

//     for (size_t i=0; i<sizeof(buf); i++)
//         buf[i] = '\0';
// }

// void guiTextArea::putch( char ch )
// {
//     if (ch == '\b' && idx > 0)
//     {
//         buf[--idx] = '\0';
//     }

//     else if (ch == '\t')
//     {
//         buf[idx++] = ' ';
//         while (idx%4 != 0)
//             buf[idx++] = ' ';
//     }

//     else if ((' ' <= ch && ch <= '~') || ch == '\n')
//     {
//         buf[idx++] = ch;
//     }

//     if (idx >= 1024)
//     {
//         return;
//     }
// }


// void guiTextArea::putstr( const char *str )
// {
//     while (*str)
//         putch(*(str++));
// }



// #include <kernel/kstring.h>
// extern guiFont *guiDefaultFont;

// void guiTextArea::draw( guiFramebuffer *dst )
// {
//     guiContainer bounds(tl, sp, margin);
//     guiRenderRect(dst, color, tl.x, tl.y, sp.x, sp.y);

//     ivec2 csr = guiRenderFont(dst, bounds, buf, tl);
//     guiRenderFont(dst, bounds, "_", csr);
// }




// void guiTextInput::putch( char ch )
// {
//     if (ch != '\n')
//     {
//         guiTextArea::putch(ch);
//         return;
//     }

//     if (this->submit != nullptr)
//     {
//         this->submit(this);
//         memset(buf, '\0', idx);
//         idx = 0;
//     }
// }


// void guiTextInput::draw( guiFramebuffer *dst )
// {
//     guiContainer bounds(tl, sp, margin);
//     guiRenderRect(dst, color, tl.x, tl.y, sp.x, sp.y);

//     ivec2 csr = guiRenderFont(dst, bounds, ">> ", tl);
//     csr = guiRenderFont(dst, bounds, buf, csr);
//     guiRenderFont(dst, bounds, "_", csr);
// }
