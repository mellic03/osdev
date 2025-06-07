#include <wm/window.hpp>
#include <wm/button.hpp>
#include <wm/frame.hpp>
#include <wm/render.hpp>

#include <kernel/log.hpp>


wm::guiWindowBar::guiWindowBar()
// :   guiElement(ivec2(0), ivec2(1))
:   guiFrameVSplit( ivec2(0), ivec2(1),
                    new guiElement(ivec2(0), ivec2(24, 128)),
                    new guiButton("X", ivec2(0), ivec2(24), []() { syslog::println("WOW!"); }),
                    48 )
{
    m_style.fillBounds = true;

}


void wm::guiWindowBar::onHoverEnter( guiMouse& )
{
    m_style.solidColor = true;
}


void wm::guiWindowBar::onHoverExit( guiMouse& )
{
    m_style.solidColor = false;
}


void wm::guiWindowBar::onMouseMove( guiMouse &ms )
{
    if (ms.dragging)
    {
        ivec2 delta = ms.xy - ms.drag.start_xy;
        m_parent->m_lpos = ms.drag.start_lpos + delta;
    }
}


void wm::guiWindowBar::onClick( guiMouse &ms )
{
    ms.drag.start_xy   = ms.xy;
    ms.drag.start_lpos = m_parent->m_lpos;
}





wm::guiWindow::guiWindow( const ivec2 &tl, const ivec2 &sp )
:   guiFrameHSplit( tl, sp,
                    new guiWindowBar(),
                    new guiElement(ivec2(0), sp),
                    32 )
{
    m_style.solidColor = true;
    // addChild(new guiButton("Yee", ivec2(25, 25), ivec2(100, 50), [](){ kpanic("Yee"); }));
    // addChild(new guiButton("Yee", ivec2(25, 25), ivec2(100, 50), [](){ }));
}


void wm::guiWindow::addChild( guiElement *E )
{
    m_bot->addChild(E);
}



void wm::guiRoot::update( const ivec2 &tl, const ivec2 &sp )
{
    m_gpos = ivec2(0, 0);
    m_gsp  = sp;

    for (auto *E: *this)
    {
        E->update(tl, sp);
    }
};


void wm::guiRoot::draw( guiFramebuffer &dst )
{
    for (auto *E: *this)
    {
        E->draw(dst);
    }
};

