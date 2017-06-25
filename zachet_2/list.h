#pragma once
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <memory>

template <typename T>
struct list;
template <typename T>
struct list_base
{
protected:
    list_base * prev, * next;
    void set_link(list_base * pos, list_base * root);
    list_base (list_base <T> const & a, list_base const * end_1);
    list_base * split_l(list_base * a)
    {
        list_base * ans = a->prev;
        a->prev = 0;
        return ans;
    }
    list_base * split_r(list_base * a)
    {
        a->prev->next = 0;
        return a;
    }
    list_base * split(list_base * a)
    {
        a->prev->next = a->next;
        a->next->prev = a->prev;
        list_base * ans = a->next;
        a->prev = 0;
        a->next = 0;
        return ans;
    }
    void merge(list_base * a, list_base * b)
    {
        a->next = b;
        b->prev = a;
    }
public:
    list_base ();
    list_base (T const &);
    ~list_base ();
    list_base (list_base const & a);
    void swap (list_base b);
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
        return const_iterator (const_cast <list_base *> (this));
    }
    const_reverse_iterator rend() const
    {
        return const_iterator (const_cast <list_base *> (next));
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
        return const_iterator (const_cast <list_base *> (next));
    }
    const_iterator end() const
    {
        return const_iterator (const_cast <list_base *> (this));
    }
    iterator insert (const_iterator to, T const & a)
    {
        list_base <T> * tmp = static_cast <list_base *> (new list <T> (a));
        const_iterator to_ = to;
        to--;
        merge(tmp, const_cast <list_base *> (to_.get()));
        merge(const_cast <list_base *> (to.get()), tmp);
        return static_cast <iterator> (tmp->next);
    }
    iterator erase (const_iterator to)
    {
        list_base * tmp = split(const_cast <list_base *> (to.get()));
        delete to.get();
        return static_cast <iterator> (tmp);
    }
    iterator erase (const_iterator a, const_iterator b)
    {
        if (a == b)
            return static_cast <iterator> (b);
        list_base * l = split_l(const_cast <list_base *> (a.get()));
        list_base * r = split_r(const_cast <list_base *> (b.get()));
        merge(l, r);
        delete a.get();
        return static_cast <iterator> (b);
    }
    void splice (const_iterator pos, list_base & other, const_iterator a, const_iterator b)
    {
        if (a == b)
            return;
        list_base * m = const_cast <list_base *> (b.get()->prev);
        list_base * l = split_l(const_cast <list_base *> (a.get()));
        list_base * r = split_r(const_cast <list_base *> (b.get()));
        const_iterator pos_ = pos;
        pos--;
        merge(l, r);
        merge(const_cast <list_base *> (pos.get()), const_cast <list_base *> (a.get()));
        merge(m, const_cast <list_base *> (pos_.get()));
    }
    list_base & operator = (list_base const & c)
    {
        swap(c);
        return (*this);
    }
    template <typename U> friend void swap(list_base <U> & a, list_base <U> & b);
};

template <typename T>
void swap(list <T> & a, list <T> & b)
{
    swap(static_cast <list_base <T>  &> (a), static_cast <list_base <T>  &> (b));
}
template <typename T>
void swap(list_base <T> & a, list_base <T> & b)
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
template <typename T> list_base <T>::
list_base ():
    prev(this),
    next(this)
{
}
template <typename T> list_base <T>::
list_base (T const & a):
    prev(this),
    next(this)
{
}
template <typename T> list_base <T>::
~list_base ()
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
template <typename T> list_base <T>::
list_base (list_base const & a):
        prev(this),
        next(this)
{
    for (iterator i = static_cast <iterator> (a.begin()); i != static_cast <iterator> (a.end()); ++i)
        insert(this, *i);
}
template <typename T> void list_base <T>::
swap (list_base b)
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
template <typename T> bool list_base <T>::
empty ()
{
    return (this == next && this == prev);
}
template <typename T> void list_base <T>::
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
template <typename T> void list_base <T>::
push_front(T const & a)
{
    insert(begin(), a);
}
template <typename T> void list_base <T>::
pop_front()
{
    erase(begin());
}
template <typename T> T const & list_base <T>::
front() const
{
    return static_cast <list <T> *> (next)->val;
}
template <typename T> T & list_base <T>::
front()
{
    return static_cast <list <T> *> (next)->val;
}
template <typename T> void list_base <T>::
push_back(T const & a)
{
    insert(end(), a);
}
template <typename T> void list_base <T>::
pop_back()
{
    erase(end());
}
template <typename T> T const & list_base <T>::
back() const
{
    return static_cast <list <T> *> (prev)->val;
}
template <typename T> T & list_base <T>::
back()
{
    return static_cast <list <T> *> (prev)->val;
}
template <typename T> struct list_base <T>::
iterator
{
private:
    list_base * val;
public:
    list_base * get()
    {
        return val;
    }
    iterator():
        val(0)
    {}
    explicit iterator(const_iterator a):
        val(const_cast <list_base *> (a.get()))
    {}
    iterator(list_base * a):
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
        return static_cast <list <T> *> (val)->val;
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
template <typename T> struct list_base <T>::
const_iterator
{
private:
    list_base const * val;
public:
    list_base * get()
    {
        return const_cast <list_base *> (val);
    }
    const_iterator(list_base * a):
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
        val = static_cast <list_base <T> *> (val->next);
        return const_cast <list_base <T> *> (val);
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
        return static_cast <list <T> const *> (val)->val;
    }
};
template <typename T> 
struct list : list_base <T>
{
public:
    T val;
    list (T const & a):
        list_base <T> :: list_base(a),
        val(a)
    {}
    list ():
        list_base <T> :: list_base()
    {}
    list (list_base <T> const & a):
        list_base <T> :: list_base(a)
    {}
    ~list ()
    {}
    list & operator = (list <T> const & c)
    {
        list_base<T>::swap(c);
        return (*this);
    }
    list & operator = (list_base <T> const & c)
    {
        list_base<T>::swap(c);
        return (*this);
    }
    template <typename U> friend void swap(list_base <U> & a, list_base <U> & b);
};
