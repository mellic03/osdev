#include <driver/video.hpp>
#include <driver/framebuffer.hpp>
#include <driver/svga.hpp>


knl::VideoInterface *kvideo2::iface;
uint8_t *kvideo2::fbmem;
uint32_t kvideo2::W;
uint32_t kvideo2::H;
uint32_t kvideo2::BPP;
uint32_t kvideo2::BYPP;
uint32_t kvideo2::Pitch;

void kvideo2::init()
{
    knl::VideoInterface *interfaces[] = {
        // SVGA::getInterface(),
        VideoFramebuffer::getInterface()
    };

    for (auto *I: interfaces)
    {
        if (I->available())
        {
            kvideo2::iface = I;
            break;
        }
    }

    kvideo2::iface->init();
    kvideo2::setMode(iface->W(), iface->H(), iface->BPP());
}


void kvideo2::flush()
{
    auto *I = kvideo2::iface;
    I->flush(0, 0, I->W(), I->H());
}


void kvideo2::setMode( uint32_t w, uint32_t h, uint32_t bpp )
{
    auto *I = kvideo2::iface;
    I->setMode(w, h, bpp);

    kvideo2::fbmem  = I->getFb();
    kvideo2::W      = I->W();
    kvideo2::H      = I->H();
    kvideo2::BPP    = I->BPP();
    kvideo2::BYPP   = I->BYPP();
    kvideo2::Pitch  = I->Pitch();
}


void kvideo2::fill( ColorRGBA8 color )
{
    auto *I = kvideo2::iface;
    I->drawRect(0, 0, I->W(), I->H(), color);
}





// void line_2d( const ivec2 &p1, const ivec2 &p2 )
// {
//     kvideo2::ColorRGBA8 color(u8vec4(255, 0, 0, 255));

//     uint8_t *dst = kvideo2::iface->getFb();
//     float m = (p1.y-p2.y) / (p1.x-p2.x); // slope
//     float c = p1.y - m*p1.x; // constant

//     // If vertical
//     if (m < -100 || m > 100)
//     {
//         if (p1.y < p2.y)
//         {
//             for (int y=p1.y; y<p2.y; y++)
//             {
//                 int idx = kvideo2::Pitch*y + kvideo2::BYPP*p1.x;
//                 *(uint32_t*)(dst+idx) = color.dword;
//             }
//         }

//         else if (p1.y > p2.y)
//         {
//             for (int y=p2.y; y<p1.y; y++)
//             {
//                 int idx = kvideo2::Pitch*y + kvideo2::BYPP*p1.x;
//                 *(uint32_t*)(dst+idx) = color.dword;
//             }
//         }
//     }

//     // if gradient is not between -1 and 1
//     else if (!(-1 <= m && m <= 1))
//     {
//         if (p1.y < p2.y)
//         {
//             for (int y=p1.y; y<p2.y; y++)
//             {
//                 int idx = kvideo2::Pitch*y + kvideo2::BYPP*(int)((y-c)/m);
//                 *(uint32_t*)(dst+idx) = color.dword;
//             }
//         }

//         else if (p1.y > p2.y)
//         {
//             for (int y=p2.y; y<p1.y; y++)
//             {
//                 int idx = kvideo2::Pitch*y + kvideo2::BYPP*(int)((y-c)/m);
//                 *(uint32_t*)(dst+idx) = color.dword;
//             }

//         }
//     }

//     // // if gradient is between -1 and 1
//     // else
//     // {
//     //     if (p1.x < p2.x)
//     //     for (int x=p1.x; x<=p2.x; x++)
//     //         pixel(x, (int)(m*x+c), stroke.x, stroke.y, stroke.z);

//     //     else if (p1.x > p2.x)
//     //     for (int x=p2.x; x<=p1.x; x++)
//     //         pixel(x, (int)(m*x+c), stroke.x, stroke.y, stroke.z);
//     // }
// }
