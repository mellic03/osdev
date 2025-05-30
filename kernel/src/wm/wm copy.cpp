// #include <kmemxx.hpp>

// #include <kernel/module.hpp>
// #include <kernel/kvideo.hpp>
// #include <kernel/event.hpp>
// #include <kernel/log.hpp>
// #include <kthread.hpp>
// #include <arch/io.hpp>

// #include <filesystem/initrd.hpp>
// #include <filesystem/vfs.hpp>

// #include <gui/gui.hpp>
// #include <bmp.hpp>
// #include <ctype.h>


// using vblankfn_t = void(*)(void*);
// static vblankfn_t vblankCallbacks[16];

// static vfsNode *msstate;
// static vfsNode *msevent;
// static vfsNode *kbevent;
// static knl::MsState msdata;
// static guiTextArea textarea({150, 150}, {250, 250});
// static guiTextInput textinput({150, 220}, {250, 50});

// // static CharDevInterface *msdev;
// // static knl::MsState      msdata;

// static void reeeeeee()
// {
//     knl::KbEvent buf[8];

//     size_t nbytes = vfs::read(kbevent, buf, 0, sizeof(buf));
//     size_t count  = nbytes / sizeof(knl::KbEvent);

//     for (size_t i=0; i<count; i++)
//     {
//         uint8_t key = buf[i].key;
//         textinput.putch(key);
//     }
// }



// void waitForVBlank()
// {
//     // Poll until not in vblank
//     while (IO::in8(0x3DA) & 0x08)
//         asm volatile ("nop");

//     // Poll until in vblank
//     while (!(IO::in8(0x3DA) & 0x08)) {  };
//         asm volatile ("nop");
// }


// static void updateInput()
// {
//     using namespace knl;
//     knl::MsEvent buf[8];
//     size_t nbytes = vfs::read(msevent, buf, 0, sizeof(buf));
//     size_t count  = nbytes / sizeof(knl::MsEvent);

//     static int state = 0;
//     static ivec2 startms = ivec2(0, 0);
//     static ivec2 starttx = ivec2(0, 0);

//     for (size_t i=0; i<count; i++)
//     {
//         auto &E = buf[i];

//         if (E.btn == MsBtn_Left && E.action == BtnAction_Down)
//         {
//             if (msdata.x == std::clamp(msdata.x, textarea.xmin(), textarea.xmax()))
//                 if (msdata.y == std::clamp(msdata.y, textarea.ymin(), textarea.ymax()))
//                 {
//                     state = 1;
//                     startms = ivec2(msdata.x, msdata.y);
//                     starttx = textarea.tl;
//                 }
//         }

//         else if (E.btn == MsBtn_Left)
//         {
//             state = 0;
//         }
//     }

//     if (state == 1)
//     {
//         ivec2 delta = ivec2(msdata.x, msdata.y) - startms;
//         textarea.tl = starttx + delta;

//         // textarea.tl.x += (msdata.x - textarea.tl.x) / 16;
//         // textarea.tl.y += (msdata.y - textarea.tl.y) / 16;
//             // textarea.onClick(textarea);

//         // std::printf("[updateInput] WOOP\n");
//     }
// }


// static guiFont font;

// void sde_main( void* )
// {
//     msstate = vfs::open("/dev/msstate");
//     msevent = vfs::open("/dev/msevent");
//     kbevent = vfs::open("/dev/kbevent");

//     textarea.onClick = [](guiContainer*) { syslog::println("textarea->onClick"); };
//     textinput.submit = [](guiTextInput *txt) { textarea.putstr(txt->buf); };

//     {
//         // BMP_File bmp(initrd::fopen("usr/share/font/cutive-w14hf20.bmp"));
//         BMP_File bmp(initrd::fopen("usr/share/font/cutive-w12hf18.bmp"));
//         font = guiFont(bmp.pixels, bmp.w, bmp.h, bmp.bpp);
//         guiSetFont(&font);
//     }

//     knl::MsState msbuf;
//     BMP_File bmp(initrd::fopen("usr/share/img/cursor.bmp"));

//     guiFramebuffer framebuffer(kvideo::backbuffer, kvideo::W, kvideo::H, kvideo::BPP);
//     guiImage cursorimg(bmp.pixels, bmp.w, bmp.h, bmp.bpp);

//     while (true)
//     {
//         if (vfs::read(msstate, &msbuf, 0, sizeof(msbuf)) == sizeof(msbuf))
//         {
//             msdata = msbuf;
//         }

//         reeeeeee();
//         updateInput();

//         // if (ready == false)
//         {
//             for (int i=0; i<16; i++)
//                 if (vblankCallbacks[i])
//                     vblankCallbacks[i](nullptr);

//             textarea.draw(&framebuffer);
//             textinput.draw(&framebuffer);

//             guiBlitImage(
//                 &framebuffer, &cursorimg,
//                 ivec2(msdata.x, msdata.y)
//             );
//             // ready = true;
//         }
    
//         waitForVBlank();

//         // if (ready && accum > 16)
//         {
//             // ready = false;
//             // accum = (accum-16) % 16;

//             kvideo::swapBuffers();
//             kthread::yield();
//         }
//     }
// }

