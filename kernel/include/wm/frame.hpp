#pragma once
#include "element.hpp"


namespace wm
{
    struct guiFrame: guiElement
    {
        using guiElement::guiElement;
        // virtual void update( const ivec2 &tl, const ivec2 &sp ) override;
    };


    struct guiFrameList: guiFrame
    {
        using guiFrame::guiFrame;
        // virtual void update( const ivec2 &tl, const ivec2 &sp ) override;
    };


    struct guiFrameHSplit: guiFrame
    {
        using guiFrame::guiFrame;

        guiElement *m_top, *m_bot;
        int m_split;

        guiFrameHSplit( const ivec2 &tl, const ivec2 &sp,
                        guiElement *top, guiElement *bot, int split );

        virtual void update( const ivec2 &tl, const ivec2 &sp ) override;
    };


    struct guiFrameVSplit: guiFrame
    {
        using guiFrame::guiFrame;

        guiElement *m_left, *m_right;
        int m_split;

        guiFrameVSplit( const ivec2 &tl, const ivec2 &sp,
                        guiElement *left, guiElement *right, int split );

        virtual void update( const ivec2 &tl, const ivec2 &sp ) override;
    };


    // struct guiFrameVSplit: guiFrame
    // {
    //     using guiFrame::guiFrame;

    //     int m_split;

    //     guiFrameVSplit( int split )
    //     :   m_split(split) {  };

    //     virtual void update() override;
    // };


}
