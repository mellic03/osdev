#include <wm/frame.hpp>
#include <wm/render.hpp>




wm::guiFrameHSplit::guiFrameHSplit( const ivec2 &tl, const ivec2 &sp,
                                    guiElement *top, guiElement *bot,
                                    int split )
:   guiFrame(tl, sp),
    m_top(top), m_bot(bot), m_split(split)
{
    addChild(m_top);
    addChild(m_bot);
}


void wm::guiFrameHSplit::update( const ivec2 &tl, const ivec2 &sp )
{
    updateBounds(tl, sp);
    m_top->update(m_gpos, ivec2(m_gsp.x, m_split));
    m_bot->update(m_gpos + ivec2(0, m_split), m_gsp - ivec2(0, m_split));
};



wm::guiFrameVSplit::guiFrameVSplit( const ivec2 &tl, const ivec2 &sp,
                                    guiElement *left, guiElement *right,
                                    int split )
:   guiFrame(tl, sp),
    m_left(left), m_right(right), m_split(split)
{
    addChild(m_left);
    addChild(m_right);
}


void wm::guiFrameVSplit::update( const ivec2 &tl, const ivec2 &sp )
{
    updateBounds(tl, sp);

    // if (m_split >= 0)
    // {
        m_left->update(m_gpos, ivec2(m_split, m_gsp.y));
        m_right->update(m_gpos + ivec2(m_split, 0), m_gsp - ivec2(m_split, 0));
    // }

    // else
    // {
    //     ivec2 splitpos = (m_gpos + m_gsp) + ivec2(m_split, 0);
    //     ivec2 sp_left = splitpos - m_gsp; 
    //     ivec2 sp_right = m_gsp - sp_left;

    //     m_left->update(m_gpos, sp_left);
    //     m_right->update(splitpos, sp_right);
    // }

};
