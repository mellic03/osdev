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
        SVGA::getInterface(),
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
