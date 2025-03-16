// #include "process.hpp"
// #include "log.hpp"
// #include "memory/memory.hpp"

// #define setjmp __builtin_setjmp
// #define longjmp __builtin_longjmp
// // #define longjmp(__buf__) __builtin_longjmp(__buf__, 1)


// using namespace idk;
// // static idk::Thread __threads[4];


// int
// idk::Scheduler::init()
// {
//     SYSLOG_BEGIN("idk::Scheduler::init");

//     this->uptime = 0;

//     SYSLOG_END
//     return 1;
// }


// void
// idk::Scheduler::update()
// {


// }



// idk::Thread::Thread( start_type start, Thread *nxt )
// {
//     next  = nxt;
//     first = false;
//     entry = start;
// }


// void
// idk::Thread::start()
// {
//     first = true;
//     this->entry(this);
// }


// void
// idk::Thread::yield()
// {
//     auto *cr = this;
//     auto *nx = this->next;

//     if (cr->first == true)
//     {
//         cr->first = false;
    
//         if (setjmp(cr->ctx) == 0)
//         {
//             nx->entry(nx);
//         }
//     }

//     else
//     {
//         if (setjmp(cr->ctx) == 0)
//         {
//             longjmp(nx->ctx, 1);
//         }
//     }
// }




// void routineB( idk::Thread *th );
// void routineA( idk::Thread *th )
// {
//     SYSLOG("A:0\n");  th->yield();
//     SYSLOG("A:1\n");  th->yield();
//     SYSLOG("A:2\n");  th->yield();
// }


// void routineB( idk::Thread *th )
// {
//     SYSLOG("B:0\n");  th->yield();
//     SYSLOG("B:1\n");  th->yield();
//     SYSLOG("B:2\n");  th->yield();
// }



// idk::Thread thread0;
// idk::Thread thread1;


// void thread_test()
// {
//     SYSLOG_BEGIN("thread_test");

//     thread0 = idk::Thread(routineA, &thread1);
//     thread1 = idk::Thread(routineB, &thread0);

//     thread0.start();

//     SYSLOG_END
// }


