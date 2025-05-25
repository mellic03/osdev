// #include <kernel/input.hpp>
// #include <kmemxx.hpp>
// #include <mutex>



// // Mouse
// // using msCallbackFn = void (*)(const knl::MsState&);
// // static msCallbackFn ms_callbacks[32];

// static std::mutex          msdataLock{false};
// static std::mutex          msCallbacksLock{false};
// static kinput::MsData      msdata;
// static kinput::MsCallbacks mscallbacks;

// void kinput::writeMsData( const kinput::MsData &data )
// {
//     std::lock_guard lock(msdataLock);
//     msdata = data;
// }

// void kinput::readMsData( kinput::MsData &data )
// {
//     std::lock_guard lock(msdataLock);
//     data = msdata;
// }

// void kinput::writeMsCallbacks( const kinput::MsCallbacks &data )
// {
//     std::lock_guard lock(msCallbacksLock);
//     mscallbacks = data;
// }

// void kinput::readMsCallbacks( kinput::MsCallbacks &data )
// {
//     std::lock_guard lock(msCallbacksLock);
//     data = mscallbacks;
// }




// // Keyboard
// // using kbCallbackFn = void (*)(const knl::KbEvent&);
// // static kbCallbackFn kb_callbacks[256];

// // static void kb_example( const knl::KbEvent &event )
// // {
// //     if (event.key == 'f')
// //     {
// //         printf("f in the chat");
// //     }
// // }


