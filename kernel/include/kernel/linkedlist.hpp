#pragma once
#include <stddef.h>

namespace knl
{
    struct LinkedListNode
    {
        LinkedListNode *prev, *next;
        LinkedListNode(): prev(nullptr), next(nullptr) {  };
        void makeLast( LinkedListNode *&root );
    };

    template <typename T>
    class LinkedList;
}



template <typename NodeType=knl::LinkedListNode>
class knl::LinkedList
{
public:

    LinkedList()
    : m_root(nullptr), m_size(0) {  };

    void insert( NodeType* );
    void remove( NodeType* );
    void makeFirst();

    size_t size() const { return (size_t)m_size; };

    struct iterator
    {
        NodeType *curr;

        iterator( NodeType *node ): curr(node) {  };
        iterator( const iterator &it ): curr(it.curr) {  };

        iterator &operator++()
        {
            curr = (NodeType*)(curr->next);
            return *this;
        };

        iterator operator++(int)
        {
            return iterator(curr->next);
        };

        bool operator == ( const iterator &rhs ) { return curr == rhs.curr; };
        bool operator != ( const iterator &rhs ) { return curr != rhs.curr; };
        NodeType *&operator*() { return curr; };
    };

    iterator begin() { return iterator(m_root); };
    iterator end()   { return iterator(nullptr); };

private:
    NodeType *m_root;
    int m_size;
};





template <typename NodeType>
void knl::LinkedList<NodeType>::insert( NodeType *node )
{
    m_size++;

    if (m_root == nullptr)
    {
        m_root = node;
        return;
    }

    NodeType *curr = m_root;
    NodeType *next = node;

    while (curr->next)
        curr = (NodeType*)(curr->next);

    curr->next = (LinkedListNode*)next;
    next->prev = (LinkedListNode*)curr;
}


template <typename NodeType>
void knl::LinkedList<NodeType>::remove( NodeType *node )
{
    NodeType *curr = m_root;

    while (curr && (curr != node))
        curr = (NodeType*)(curr->next);

    if (curr == nullptr)
        return;

    curr->makeLast(m_root);
    curr->prev->next = nullptr;
    curr->prev = nullptr;
    
    m_size--;
}

