#include <gui/gui.hpp>



guiContainer::guiContainer( const ivec2 &corner, const ivec2 &span, const ivec4 &mgn )
:   tl(corner), sp(span), margin(mgn) {  };


guiContainer::guiContainer( int x, int y, int w, int h )
:   tl(x, y), sp(w, h), margin(5, 5, 5, 5) {  };



guiButton::guiButton( const char *name, const ivec2 &cnr, const ivec2 &spn )
:   guiContainer(cnr, spn)
{
    kmemset(label, '\0', sizeof(label));
    strncpy(label, name, sizeof(label)-1);
}

void guiButton::draw( guiFramebuffer *fb )
{
    guiRenderRect(fb, {50, 200, 200, 255}, tl.x, tl.y, sp.x, sp.y);
}





guiTextArea::guiTextArea( const ivec2 &corner, const ivec2 &span, const ivec4 &mgn )
:   guiContainer(corner, span, mgn)
{
    idx = 0;
    kmemset(buf, '\0', sizeof(buf));
}

void guiTextArea::putch( char ch )
{
    if (ch == '\b')
    {
        buf[--idx] = '\0';
    }

    else if (ch == '\t')
    {
        buf[idx++] = ' ';
        while (idx%4 != 0)
            buf[idx++] = ' ';
    }

    else if ((' ' <= ch && ch <= '~') || ch == '\n')
    {
        buf[idx++] = ch;
    }

    if (idx >= 1024)
    {
        idx = 0;
    }
}

void guiTextArea::draw( guiFramebuffer *fb )
{
    guiContainer bounds(tl, sp, ivec4(0));
    guiRenderRect(fb, {100, 100, 100, 255}, tl.x, tl.y, sp.x, sp.y);
    ivec2 csr = guiRenderFont(fb, bounds, buf, tl);
    csr = guiRenderFont(fb, bounds, "_", csr);

}

