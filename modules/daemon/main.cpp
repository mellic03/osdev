#include <driver/interface.hpp>
#include <sym/sym.hpp>

#include <kernel/event.hpp>
#include <kernel/ringbuffer.hpp>
#include <kernel/tty.hpp>
#include <kernel/kscancode.h>
#include <kmemxx.hpp>

#include <gui/gui.hpp>
#include <cringe/bmp.hpp>
#include <ctype.h>


using vblankfn_t = void(*)(void*);
static vblankfn_t vblankCallbacks[16];

static CharDevInterface *msdev;
static knl::MsState      msdata;


static void renderCursor( BMP_File &bmp )
{
    kvideo::blit(
        ivec2(msdata.x, msdata.y),
        (uint8_t*)(bmp.data),
        bmp.w, bmp.h,
        ivec2(0, 0), ivec2(bmp.w, bmp.h)
    );
}


static void sde_main( void* )
{
    BMP_File bmp(std::fopen("usr/share/img/cursor.bmp"));
    msdev = (CharDevInterface*)usrknl::findModule(ModuleType_Device, DeviceType_Mouse);

    uint64_t prev_time = 0;
    uint64_t curr_time = 0;
    uint64_t accum = 0;
    bool ready = false;

    while (true)
    {
        curr_time = std::clock();
        accum += (curr_time - prev_time);
        prev_time = curr_time;
        // std::printf("accum: %lu\n", accum);

        msdev->read(&msdata, sizeof(msdata));
    
        if (ready == false)
        {
            for (int i=0; i<16; i++)
                if (vblankCallbacks[i])
                    vblankCallbacks[i](nullptr);
            renderCursor(bmp);
            ready = true;
        }
    
        if (ready && accum > 16)
        {
            ready = false;
            accum = (accum-16) % 16;

            kvideo::swapBuffers();
            kthread::yield();
        }
    }
}


static int sde_listen( void(*callback)(void*) )
{
    for (int i=0; i<16; i++)
    {
        if (!vblankCallbacks[i])
        {
            vblankCallbacks[i] = callback;
            return i;
        }
    }
    return -1;
}


static int sde_forget( int idx )
{
    if (vblankCallbacks[idx])
    {
        vblankCallbacks[idx] = nullptr;
        return idx;
    }

    return -1;
}



extern "C"
ModuleInterface *init( ksym::ksym_t *sym )
{
    ksym::loadsym(sym);
    kmemset(vblankCallbacks, 0, sizeof(vblankCallbacks));

    auto *daemon = (DaemonInterface*)std::malloc(sizeof(DaemonInterface));
    *daemon = {
        .modtype  = ModuleType_Daemon,
        .basetype = DaemonType_System,
        .main     = sde_main,
        .start    = nullptr,
        .stop     = nullptr,
        .reload   = nullptr,
        .status   = nullptr,
        .listen   = sde_listen,
        .forget   = sde_forget
    };

    kmemset<char>(daemon->signature, '\0', sizeof(daemon->signature));
    kmemcpy<char>(daemon->signature, "shitde", 6);

    return (ModuleInterface*)daemon;
}
