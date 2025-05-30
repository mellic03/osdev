#include <driver/interface.hpp>
#include <sym/sym.hpp>

#include <arch/io.hpp>
#include <kernel/event.hpp>
#include <kernel/ringbuffer.hpp>
#include <kernel/kscancode.h>
#include <kernel/log.hpp>
#include <kmemxx.hpp>

#include <gui/gui.hpp>
#include <bmp.hpp>
#include <ctype.h>
#include <new>


using vblankfn_t = void(*)(void*);
static vblankfn_t vblankCallbacks[16];

static vfsNode *msstate;
static vfsNode *msevent;
static knl::MsState msdata;

static vfsNode *kbevent;
static guiTextArea *textarea;
static guiTextInput *textinput;

// static CharDevInterface *msdev;
// static knl::MsState      msdata;

static void reeeeeee()
{
    knl::KbEvent buf[8];

    size_t nbytes = uvfs::read(kbevent, buf, 0, sizeof(buf));
    size_t count  = nbytes / sizeof(knl::KbEvent);

    for (size_t i=0; i<count; i++)
    {
        uint8_t key = buf[i].key;
        textarea->putch(key);
    }
}



void waitForVBlank()
{
    // Poll until not in vblank
    while (IO::in8(0x3DA) & 0x08)
        asm volatile ("nop");

    // Poll until in vblank
    while (!(IO::in8(0x3DA) & 0x08)) {  };
        asm volatile ("nop");
}


static int state;
static ivec2 startms;
static ivec2 starttx;

static void updateInput()
{
    using namespace knl;
    knl::MsEvent buf[8];
    size_t nbytes = uvfs::read(msevent, buf, 0, sizeof(buf));
    size_t count  = nbytes / sizeof(knl::MsEvent);

    for (size_t i=0; i<count; i++)
    {
        auto &E = buf[i];

        if (E.btn == MsBtn_Left && E.action == BtnAction_Down)
        {
            if (msdata.x == std::clamp(msdata.x, textarea->xmin(), textarea->xmax()))
                if (msdata.y == std::clamp(msdata.y, textarea->ymin(), textarea->ymax()))
                {
                    textarea->onClick(textarea);
                    state = 1;
                    startms = ivec2(msdata.x, msdata.y);
                    starttx = textarea->tl;
                }
        }

        else if (E.btn == MsBtn_Left)
        {
            state = 0;
        }
    }

    if (state == 1)
    {
        ivec2 delta = ivec2(msdata.x, msdata.y) - startms;
        textarea->tl = starttx + delta;

        // textarea->tl.x += (msdata.x - textarea->tl.x) / 16;
        // textarea->tl.y += (msdata.y - textarea->tl.y) / 16;
            // textarea->onClick(textarea);

        // std::printf("[updateInput] WOOP\n");
    }
}



static void sde_main( void *font_addr )
{
    msstate = uvfs::open("/dev/msstate");
    msevent = uvfs::open("/dev/msevent");
    kbevent = uvfs::open("/dev/kbevent");

    state = 0;
    startms = ivec2(0, 0);
    starttx = ivec2(0, 0);

    {
        auto &bmp = *(BMP_File*)font_addr;
        guiFont font(bmp.pixels, bmp.w, bmp.h, bmp.bpp);
        guiSetFont(&font);
    }

    knl::MsState msbuf;
    BMP_File bmp(std::fopen("usr/share/img/cursor.bmp"));

    guiFramebuffer framebuffer(kvideo::backbuffer, kvideo::W, kvideo::H, kvideo::BPP);
    guiImage cursorimg(bmp.pixels, bmp.w, bmp.h, bmp.bpp);

    // uint64_t prev_time = 0;
    // uint64_t curr_time = 0;
    // uint64_t accum = 0;
    // bool ready = false;

    while (true)
    {
        // curr_time = std::clock();
        // accum += (curr_time - prev_time);
        // prev_time = curr_time;
        // std::printf("accum: %lu\n", accum);

        if (uvfs::read(msstate, &msbuf, 0, sizeof(msbuf)) == sizeof(msbuf))
        {
            msdata = msbuf;
        }

        reeeeeee();
        updateInput();

        // if (ready == false)
        {
            for (int i=0; i<16; i++)
                if (vblankCallbacks[i])
                    vblankCallbacks[i](nullptr);

            textarea->draw(&framebuffer);

            guiBlitImage(
                &framebuffer, &cursorimg,
                ivec2(msdata.x, msdata.y)
            );
            // ready = true;
        }
    
        waitForVBlank();

        // if (ready && accum > 16)
        {
            // ready = false;
            // accum = (accum-16) % 16;

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
    textarea = new (std::malloc(sizeof(guiTextArea))) guiTextArea({100, 100}, {250, 250});
    textarea->onClick = [](guiContainer*) { syslog::println("textarea->onClick"); };
    // textinput->onSubmit = [](guiTextInput*) {  };

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
