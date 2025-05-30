// #include <kernel/event.hpp>
// #include <kernel/ringbuffer.hpp>
// #include <kmemxx.hpp>
// #include <string.h>
// #include <mutex>

// using namespace knl;


// static RingBuffer<MsState, 32> msStateBuffer;
// static RingBuffer<MsEvent, 64> msEventBuffer;
// static RingBuffer<KbEvent, 64> kbEventBuffer;


// bool knl::readMsState( MsState &state )
// {
//     if (msStateBuffer.empty())
//         return false;
//     state = msStateBuffer.pop_front();
//     return true;
// }

// void knl::writeMsState( const MsState &state )
// {
//     if (msStateBuffer.full() == false)
//         msStateBuffer.push_back(state);
// }


// bool knl::readMsEvent( MsEvent &event )
// {
//     if (msEventBuffer.empty())
//         return false;
//     event = msEventBuffer.pop_front();
//     return true;
// }

// void knl::writeMsEvent( const MsEvent &event )
// {
//     if (msEventBuffer.full() == false)
//         msEventBuffer.push_back(event);
// }



// bool knl::readKbEvent( KbEvent &event )
// {
//     if (kbEventBuffer.empty())
//         return false;
//     event = kbEventBuffer.pop_front();
//     return true;
// }

// void knl::writeKbEvent( const KbEvent &event )
// {
//     if (kbEventBuffer.full() == false)
//         kbEventBuffer.push_back(event);
// }





// using msEventCallback = void (*)(const MsEvent&);
// using kbEventCallback = void (*)(const KbEvent&);

// static msEventCallback msCallbacks[32];
// static kbEventCallback kbCallbacks[32];

// __attribute__((constructor))
// static void initCallbacks()
// {
//     memset(msCallbacks, 0, sizeof(msCallbacks));
//     memset(kbCallbacks, 0, sizeof(kbCallbacks));
// }


// int knl::listenMsEvent( void (*callback)(const MsEvent&) )
// {
//     for (int i=0; i<32; i++)
//     {
//         if (!msCallbacks[i])
//         {
//             msCallbacks[i] = callback;
//             return i;
//         }
//     }

//     return -1;
// }


// int knl::listenKbEvent( void (*callback)(const KbEvent&) )
// {
//     for (int i=0; i<32; i++)
//     {
//         if (!kbCallbacks[i])
//         {
//             kbCallbacks[i] = callback;
//             return i;
//         }
//     }

//     return -1;
// }


// void knl::emitMsEvent( const MsEvent &event )
// {
//     for (int i=0; i<32; i++)
//         if (msCallbacks[i])
//             msCallbacks[i](event);
// }

// void knl::emitKbEvent( const KbEvent &event )
// {
//     for (int i=0; i<32; i++)
//         if (kbCallbacks[i])
//             kbCallbacks[i](event);
// }



// void knl::forgetMsEvent( int idx )
// {
//     msCallbacks[idx] = nullptr;
// }


// void knl::forgetKbEvent( int idx )
// {
//     kbCallbacks[idx] = nullptr;
// }


