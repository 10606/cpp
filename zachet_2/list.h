#pragma once
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <memory>

template <typename T>
struct node;
template <typename T>
struct list
{
protected:
    list * prev, * next;
    list * split_l(list * a)
    {
        list * ans = a->prev;
        a->prev = 0;
        return ans;
    }
    list * split_r(list * a)
    {
        a->prev->next = 0;
        return a;
    }
    list * split(list * a)
    {
        a->prev->next = a->next;
        a->next->prev = a->prev;
        list * ans = a->next;
        a->prev = 0;
        a->next = 0;
        return ans;
    }
    void merge(list * a, list * b)
    {
        a->next = b;
        b->prev = a;
    }
public:
    list ();
    list (T const &);
    ~list ();
    list (list const & a);
    void swap (list b);
    bool empty ();
    void clear();
    void push_back(T const & a);
    void push_front(T const & a);
    void pop_back();
    void pop_front();
    T const & back() const;
    T const & front() const;
    T & back();
    T & front();
    struct iterator;
    struct const_iterator;
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    reverse_iterator rbegin()
    {
        return iterator (this);
    }
    reverse_iterator rend()
    {
        return iterator (next);
    }
    const_reverse_iterator rbegin() const
    {
        return const_iterator (const_cast <list *> (this));
    }
    const_reverse_iterator rend() const
    {
        return const_iterator (const_cast <list *> (next));
    }
    iterator begin()
    {
        return iterator (next);
    }
    iterator end()
    {
        return iterator (this);
    }
    const_iterator begin() const
    {
        return const_iterator (const_cast <list *> (next));
    }
    const_iterator end() const
    {
        return const_iterator (const_cast <list *> (this));
    }
    iterator insert (const_iterator to, T const & a)
    {
        list <T> * tmp = static_cast <list *> (new node <T> (a));
        const_iterator to_ = to;
        to--;
        merge(tmp, const_cast <list *> (to_.get()));
        merge(const_cast <list *> (to.get()), tmp);
        return static_cast <iterator> (tmp->next);
    }
    iterator erase (const_iterator to)
    {
        list * tmp = split(const_cast <list *> (to.get()));
        delete to.get();
        return static_cast <iterator> (tmp);
    }
    iterator erase (const_iterator a, const_iterator b)
    {
        if (a == b)
            return static_cast <iterator> (b);
        list * l = split_l(const_cast <list *> (a.get()));
        list * r = split_r(const_cast <list *> (b.get()));
        merge(l, r);
        delete a.get();
        return static_cast <iterator> (b);
    }
    void splice (const_iterator pos, list & other, const_iterator a, const_iterator b)
    {
        if (a == b)
            return;
        list * m = const_cast <list *> (b.get()->prev);
        list * l = split_l(const_cast <list *> (a.get()));
        list * r = split_r(const_cast <list *> (b.get()));
        const_iterator pos_ = pos;
        pos--;
        merge(l, r);
        merge(const_cast <list *> (pos.get()), const_cast <list *> (a.get()));
        merge(m, const_cast <list *> (pos_.get()));
    }
    list & operator = (list const & c)
    {
        swap(c);
        return (*this);
    }
    template <typename U> friend void swap(list <U> & a, list <U> & b);
};

template <typename T>
void swap(list <T> & a, list <T> & b)
{
    if (a.empty() && b.empty())
    {
        b.next = &b;
        b.prev = &b;
        a.next = &a;
        a.prev = &a;
        return;
    }
    if (a.empty())
    {
        a.next = b.next;
        a.prev = b.prev;
        b.next = &b;
        b.prev = &b;
    }
    else if (b.empty())
    {
        b.next = a.next;
        b.prev = a.prev;
        a.next = &a;
        a.prev = &a;
    }
    else
    {
        std::swap(a.next, b.next);
        std::swap(a.prev, b.prev);
    }
    b.prev->next = &b;
    b.next->prev = &b;
    a.prev->next = &a;
    a.next->prev = &a;
}
template <typename T> list <T>::
list ():
    prev(this),
    next(this)
{
}
template <typename T> list <T>::
list (T const & a):
    prev(this),
    next(this)
{
}
template <typename T> list <T>::
~list ()
{
    if (empty())
        return;
    if (next)
        next->prev = 0;
    if (prev)
        prev->next = 0;
    if (next)
    {
        delete next;
    }
}
template <typename T> list <T>::
list (list const & a):
        prev(this),
        next(this)
{
    for (iterator i = static_cast <iterator> (a.begin()); i != static_cast <iterator> (a.end()); ++i)
        insert(this, *i);
}
template <typename T> void list <T>::
swap (list b)
{
    if (b.empty())
    {
        clear();
        return;
    }
    std::swap(next, b.next);
    std::swap(prev, b.prev);
    if (b.prev)
        b.prev->next = &b;
    if (b.next)
        b.next->prev = &b;
    if (prev)
        prev->next = this;
    if (next)
        next->prev = this;
}
template <typename T> bool list <T>::
empty ()
{
    return (this == next && this == prev);
}
template <typename T> void list <T>::
clear()
{
    if (empty())
        return;
    next->prev = 0;
    if (prev)
        prev->next = 0;
    delete next;
    next = this;
    prev = this;
}
template <typename T> void list <T>::
push_front(T const & a)
{
    insert(begin(), a);
}
template <typename T> void list <T>::
pop_front()
{
    erase(begin());
}
template <typename T> T const & list <T>::
front() const
{
    return static_cast <node <T> *> (next)->val;
}
template <typename T> T & list <T>::
front()
{
    return static_cast <node <T> *> (next)->val;
}
template <typename T> void list <T>::
push_back(T const & a)
{
    insert(end(), a);
}
template <typename T> void list <T>::
pop_back()
{
    erase(end());
}
template <typename T> T const & list <T>::
back() const
{
    return static_cast <node <T> *> (prev)->val;
}
template <typename T> T & list <T>::
back()
{
    return static_cast <node <T> *> (prev)->val;
}
template <typename T> struct list <T>::
iterator
{
private:
    list * val;
public:
    list * get()
    {
        return val;
    }
    iterator():
        val(0)
    {}
    explicit iterator(const_iterator a):
        val(const_cast <list *> (a.get()))
    {}
    iterator(list * a):
        val(a)
    {}
    iterator & operator ++ ()
    {
        val = val->next;
        return *this;
    }
    iterator & operator -- ()
    {
        val = val->prev;
        return *this;
    }
    iterator operator ++ (int)
    {
        val = val->next;
        return val->prev;
    }
    iterator operator -- (int)
    {
        val = val->prev;
        return val->next;
    }
    T & operator * ()
    {
        return static_cast <node <T> *> (val)->val;
    }
    bool operator != (iterator a)
    {
        return a.val != val;
    }
    bool operator == (iterator a)
    {
        return a.val == val;
    }
};
template <typename T> struct list <T>::
const_iterator
{
private:
    list const * val;
public:
    list * get()
    {
        return const_cast <list *> (val);
    }
    const_iterator(list * a):
        val(a)
    {}
    const_iterator():
        val(0)
    {}
    const_iterator(iterator a):
        val(a.get())
    {}
    const_iterator operator ++ ()
    {
        val = static_cast <list <T> *> (val->next);
        return const_cast <list <T> *> (val);
    }
    const_iterator operator -- ()
    {
        val = val->prev;
        return val;
    }
    const_iterator operator ++ (int)
    {
        val = val->next;
        return val->prev;
    }
    const_iterator operator -- (int)
    {
        val = val->prev;
        return val->next;
    }
    bool operator != (const_iterator a)
    {
        return a.val != val;
    }
    bool operator == (const_iterator a)
    {
        return a.val == val;
    }
    T const & operator * ()
    {
        return static_cast <node <T> const *> (val)->val;
    }
};
template <typename T> 
struct node : list <T>
{
public:
    T val;
    node (T const & a):
        list <T> :: list(a),
        val(a)
    {}
    /*
    node ():
        list <T> :: list()
    {}
    node (list <T> const & a):
        list <T> :: list(a)
    {}
    
    ~node ()
    {}
    node & operator = (node <T> const & c)
    {
        list<T>::swap(c);
        return (*this);
    }
    node & operator = (list <T> const & c)
    {
        list<T>::swap(c);
        return (*this);
    }
    template <typename U> friend void swap(list <U> & a, list <U> & b);
    */
};
