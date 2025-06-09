#include <kernel/boot_limine.hpp>
#include <driver/framebuffer.hpp>
#include <kmemxx.hpp>

#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <kernel/log.hpp>



class VideoInterfaceFramebuffer: public knl::VideoInterface
{
private:
    uint8_t *m_front, *m_back;

public:
    virtual bool available() final;
    virtual void init() final;
    virtual void setMode( uint32_t w, uint32_t h, uint32_t bpp ) final;
    virtual void flush( uint32_t x, uint32_t y, uint32_t w, uint32_t h ) final;
    virtual uint8_t *getFb() final;
    virtual void drawRect( int x, int y, int w, int h, kvideo2::ColorRGBA8 color ) final;

};


knl::VideoInterface *VideoFramebuffer::getInterface()
{
    static VideoInterfaceFramebuffer I;
    return (knl::VideoInterface*)(&I);
}




bool VideoInterfaceFramebuffer::available()
{
    return (limine_res.fb != nullptr);
}


void VideoInterfaceFramebuffer::init()
{
    auto *res = limine_res.fb;
    auto *fb  = res->framebuffers[0];

    m_mode.w     = fb->width;
    m_mode.h     = fb->height;
    m_mode.bpp   = fb->bpp;
    m_mode.pitch = fb->pitch;


    using namespace VMM;

    size_t nbytes = m_mode.pitch * m_mode.h;
    m_front = (uint8_t*)(fb->address);
    m_back  = (uint8_t*)VMM::alloc(nbytes, PAGE_PRESENT|PAGE_WRITE);
    // m_back = (uint8_t*)VMM::alloc(nbytes, PAGE_PRESENT|PAGE_WRITE|PAGE_PCD|PAGE_PWT);

    syslog log("VideoInterfaceFramebuffer::init");
    log("fb count:    %u",    res->framebuffer_count);
    // log("address:     0x%lx", fb->address);
    log("width:       %u",    fb->width);
    log("height:      %u",    fb->height);
    log("pitch:       %u",    fb->pitch);
    log("bpp:         %u",    fb->bpp);
    // log("mem model:   %u",    fb->memory_model);
    // log("edid:        0x%lx", fb->edid);
    // log("edid_size:   %lu",   fb->edid_size);
    // log("mode_count:  %lu",   fb->mode_count);
    // log("rmask_size:  %u",    fb->red_mask_size);
    // log("rmask_shift: %u",    fb->red_mask_shift);
    // log("gmask_size:  %u",    fb->green_mask_size);
    // log("gmask_shift: %u",    fb->green_mask_shift);
    // log("bmask_size:  %u",    fb->blue_mask_size);
    // log("bmask_shift: %u",    fb->blue_mask_shift);


}


void VideoInterfaceFramebuffer::setMode( uint32_t, uint32_t, uint32_t )
{
    // m_mode = {w, h, bpp, w*bpp};
}


void VideoInterfaceFramebuffer::flush( uint32_t x, uint32_t y, uint32_t w, uint32_t h )
{
    if (x || y || w || h)
    {
        
    }

    size_t nbytes = m_mode.pitch * m_mode.h;
    kmemcpy<uint128_t>(m_front, m_back, nbytes);
    kmemset<uint128_t>(m_back, 0, nbytes);
}


uint8_t *VideoInterfaceFramebuffer::getFb()
{
    return m_back;
}



#include <cpu/cpu.hpp>

void VideoInterfaceFramebuffer::drawRect( int x, int y, int w, int h, kvideo2::ColorRGBA8 color )
{
    uint8_t *dst   = this->getFb();
    uint32_t bypp  = BYPP();
    uint32_t pitch = Pitch();
    // uint8_t a = color.a;
    // uint8_t ia = 255 - color.a;

    for (int i=y; i<y+h; i++)
    {
        // int idx = pitch*i + bypp*x;
        // CPU::stos32(dst+idx, color, w);

        for (int j=x; j<x+w; j++)
        {
            int idx = pitch*i + bypp*j;
        
            for (uint32_t k=0; k<bypp; k++)
            {
                dst[idx + k] = color.rgba[k];
            }
        }
    }
}
