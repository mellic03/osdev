#pragma once
#include <util/linkedlist.hpp>
#include <smp/lock.hpp>
#include <mutex>


namespace knl
{
    template <typename T = void*>
    struct EventListener: public knl::LinkedListNode
    {
        virtual void onEvent( T ) = 0;
    };


    template <typename T = void*>
    class EventEmitter
    {
    private:
        knl::SpinLock<false> m_lock;
        knl::LinkedList<EventListener<T>> m_listeners;

    public:

        void emit( T data )
        {
            std::lock_guard lock(m_lock);

            for (auto *node: m_listeners)
            {
                node->onEvent(data);
            }
        }

        void listen( EventListener<T> *E )
        {
            std::lock_guard lock(m_lock);
            m_listeners.insert(E);
        }

        void forget( EventListener<T> *E )
        {
            std::lock_guard lock(m_lock);
            m_listeners.remove(E);
        }

    };

}



