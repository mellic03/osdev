// #pragma once
// #include "data_structures/inplace_vector.hpp"

// namespace idk
// {
//     static constexpr size_t MAX_PROCESSES = 32;
//     using jmp_buf    = void*[5];
//     using process_fn = void (*)();
        
//     struct Thread
//     {
//         // using start_type = void(*)();
//         using start_type = void(*)(idk::Thread*);
//         bool first;
//         Thread *next;
//         jmp_buf ctx;
//         start_type entry;
//         // uint8_t *stack;
    
//         Thread() {  };
//         Thread( start_type, Thread *next = nullptr );

//         void start();
//         void yield();
//     };

//     class Scheduler;
// }


// class idk::Scheduler
// {
// private:

// public:
//     uint64_t uptime;

//     int  init();
//     void update();
// };



// void thread_test();


