#pragma once

#include <stddef.h>
#include <stdint.h>
#include <../../external/limine/limine.h>
// #include <limine.h>


struct ck_Rect
{
    int x, y, w, h;
};


struct ck_Color
{
    uint8_t r, g, b, a;

    // ck_Color(int r, int g, int b, int a)
    // :   r(r), g(g), b(b), a(a) {  };

    // ck_Color(float r, float g, float b, float a)
    // :   r(r), g(g), b(b), a(a) {  };

    // ck_Color(double r, double g, double b, double a)
    // :   r(r), g(g), b(b), a(a) {  };

};


void fb_init( limine_framebuffer& );
void fb_blit( ck_Rect &src, ck_Rect &dst );
void fb_rect( const ck_Rect&, const ck_Color& );


