#pragma once
#include <cringe/vec.hpp>
#include <kmemxx.hpp>
#include <string.h>

struct guiFramebuffer;


struct guiContainer
{
    ivec2 tl, sp;
    ivec4 margin;

    guiContainer( const ivec2 &cnr=ivec2(0), const ivec2 &spn=ivec2(256),
                  const ivec4 &mgn=ivec4(15));

    guiContainer( int x, int y, int w, int h );

    virtual void draw( guiFramebuffer* ) {  };

    int xmin() const { return tl.x        + margin[0]; }
    int xmax() const { return tl.x + sp.x - margin[1]; }
    int ymin() const { return tl.y        + margin[2]; }
    int ymax() const { return tl.y + sp.y - margin[3]; }
};


struct guiButton: public guiContainer
{
    char label[32];
    void (*onClick)() = nullptr;

    guiButton( const char *name, const ivec2 &cnr, const ivec2 &spn );
    virtual void draw( guiFramebuffer* ) final;

};


struct guiTextArea: public guiContainer
{
    char buf[1024];
    int idx;
    guiTextArea( const ivec2 &cnr=ivec2(0), const ivec2 &spn=ivec2(256),
                 const ivec4 &mgn=ivec4(15));
                
    void putch( char ch );

    using guiContainer::guiContainer;
    virtual void draw( guiFramebuffer* ) final;
};

