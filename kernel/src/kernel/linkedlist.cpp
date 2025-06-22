#include <util/linkedlist.hpp>




void knl::LinkedListNode::makeLast( LinkedListNode *&root )
{
    if (this->next == nullptr)
        return;

    LinkedListNode *last = this;
    LinkedListNode *F = root;
    LinkedListNode *L = this->prev;
    LinkedListNode *R = this->next;

    if (L) L->next = R;
    if (R) R->prev = L;

    while (last->next)
        last = last->next;

    last->next = this;
    this->prev = last;
    this->next = nullptr;

    if (F == this)
    {
        root = R;
    }
}


