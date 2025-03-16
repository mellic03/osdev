#pragma once
#include "./font.hpp"
#include "./color.hpp"
#include <idk/vec.hpp>
#include "system/terminal/terminal.hpp"
#include "system/data_structures/inplace_array.hpp"
#include "system/data_structures/inplace_vector.hpp"


namespace idk
{
    class Video;

    // struct WinFrameTransform;
    // struct WinFrameStyle;
    
    class WinFrameBase;
    class WinFrameSolid;
    class WinFrameTerminal;
}


// struct idk::WinFrameTransform
// {
//     vec3 tl = vec3(0.0f);
//     vec2 sp = vec2(1.0f);

//     WinFrameTransform()
//     :   WinFrameTransform(vec2(0.0f), vec2(1.0f)) {  };

//     WinFrameTransform( const vec2 &p, const vec2 &s )
//     :   tl(vec3(p, 0.9999f)), sp(s) {  };

//     WinFrameTransform( const vec3 &p, const vec2 &s )
//     :   tl(p), sp(s) {  };
// };


// struct idk::WinFrameStyle
// {
//     uvec4 fg;
//     uvec4 bg;
//     ivec2 border;
// };


class idk::WinFrameBase
{
private:
    WinFrameBase *cbuf[8];

protected:

public:
    // WinFrameTransform m_local;
    // WinFrameTransform m_world;
    // WinFrameStyle m_style;
    ivec2 m_tl;
    ivec2 m_sp;
    uvec4 m_bg;
    float m_depth;

    WinFrameBase *m_parent;
    idk::inplace_vector<WinFrameBase*> m_children;

    WinFrameBase( const vec2 &tl, const vec2 &sp );
    // WinFrameBase( const vec2 &corner, const vec2 &span, const WinFrameStyle& );
    virtual ~WinFrameBase() = default;

    void giveChild( WinFrameBase* );

    virtual void render( idk::Video &video ) = 0;

};



class idk::WinFrameSolid: public idk::WinFrameBase
{
private:

public:
    using WinFrameBase::WinFrameBase;
    virtual void render( idk::Video &video ) override;

};





class idk::WinFrameTerminal: public idk::WinFrameBase
{
private:
    idk::Terminal   *m_tty;
    idk::FontBuffer *m_font;

public:
    WinFrameTerminal( idk::Terminal *tty, idk::FontBuffer *font );
    virtual void render( idk::Video &video ) override;
};







// struct idk::VWindow
// {
//     const int ID;
//     bool dirty;

//     ivec2 corner;
//     ivec2 extents;
//     uvec4 bg;

//     int &x, &y;
//     int &w, &h;
//     int8_t depth;

//     idk::Terminal   *term;
//     idk::FontBuffer *font;

//     VWindow *children[4] = {nullptr, nullptr, nullptr, nullptr};

//     VWindow( int id, const ivec2 &cnr, const ivec2 &ext, const uvec4 &color );

//     void setpos( const ivec2 &pos );
//     void translate( const ivec2 &delta );
//     void resize( const ivec2 &size );
// };


