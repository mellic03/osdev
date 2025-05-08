#pragma once

template <size_t cap, typename T>
struct gx_vector
{
    T  data[cap];
    T *top = data;
    size_t size;

    void push( const T &value )
    {
        *(top++) = value;
    }

    void pop()
    {
        *(--top).~T();
    }

    T &back() { return *(top-1); }
    T &front() { return data[0];  }
    T &operator[] (int i) { return *(data + i); }

    const T &back() const { return *(top-1); }
    const T &front() const { return data[0];  }
    const T &operator[] (int i) const { return *(data + i); }
};


template <typename T, size_t cap>
struct gx_deque
{
public:
    struct Node
    {
        int   idx;
        Node *prev;
        Node *next;
        T     data;
    
        Node( int id=-1, Node *p=nullptr, Node *n=nullptr )
        :   idx(id), prev(p), next(n)
        {

        }
    };

private:
    Node  nodes[cap];
    Node *m_head = nullptr;
    Node *m_tail = nullptr;
    int   m_size = 0;

    Node *alloc_node( Node *prev=nullptr, Node *next=nullptr )
    {
        for (size_t i=0; i<cap; i++)
        {
            if (nodes[i].idx == -1)
            {
                nodes[i] = Node(i, prev, next);
                return &nodes[i];
            }
        }

        return nullptr;
    }

    void free_node( Node *node )
    {
        *node = Node(-1);
    }


public:
    size_t size;

    gx_deque()
    {
        for (size_t i=0; i<cap; i++)
        {
            nodes[i] = Node(-1, nullptr, nullptr);
        }
    }

    void push_back( const T &value )
    {
        if (m_size == 0)
        {
            m_head = alloc_node(nullptr, nullptr);
            m_tail = m_head;
        }

        else if (m_size == 1)
        {
            m_tail = alloc_node(m_head, nullptr);
        }

        Node *left = m_tail;
        m_tail->next = alloc_node(left, nullptr);
        m_tail = m_tail->next;
        m_tail->data = value;
    
        m_size += 1;
    }

    void pop_back()
    {
        if (m_tail)
        {
            Node *temp = m_tail;
            m_tail = m_tail->prev;
            free_node(temp);
        }

        m_size -= 1;
    }

    void push_front( const T &value )
    {
        if (m_size == 0)
        {
            m_tail = alloc_node(m_head, nullptr);
            m_head = m_tail;
        }
    
        Node *right = m_head;
        m_head = alloc_node(nullptr, m_head);
        m_head->next = right;
        m_head->data = value;

        m_size += 1;
    }

    void pop_front()
    {
        if (m_head)
        {
            Node *temp = m_head;
            m_head = m_head->next;
            free_node(temp);
        }
    
        m_size -= 1;
    }

    void erase( const T &value )
    {
        Node *curr = m_head;
    
        while (curr)
        {
            if (curr->data == value)
            {
                break;
            }
            curr = curr->next;
        }

        if (curr)
        {
            Node *L = curr->prev;
            Node *R = curr->next;
            if (L) { L->next = R; }
            if (R) { R->prev = L; }
        }
    }

    struct iterator;
    iterator begin() { return iterator(m_head); };
    iterator end()   { return iterator(m_tail); };

};




template <typename T, size_t N>
struct gx_deque<T, N>::iterator
{
    Node *curr;

    iterator( Node *node ): curr(node) {  };
    iterator( const iterator &other ): curr(other.curr) {  };

    iterator &operator++()
    {
        iterator temp = *this;
        curr = curr->next;
        return *temp;
    };

    iterator operator++(int)
    {
        return iterator(curr->next);
    };

    bool operator == ( const iterator &rhs ) { return curr == rhs.curr; };
    bool operator != ( const iterator &rhs ) { return curr != rhs.curr; };
    T &operator * () { return curr->data; };

};



