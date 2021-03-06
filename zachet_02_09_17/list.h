#pragma once
#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <memory>

template <typename T>
struct list;

template <typename T>
struct global
{
    int cnt;
    bool valid;
    list <T> * type;
    global(list <T> * type_):
        cnt(1),
        valid(1),
        type(type_)
    {}
};

template <typename T>
struct node;
template <typename T>
struct list
{
    template <typename U>
    friend struct node;
protected:
    global <T> * gpointer;
    list * prev, * next, * root;
    bool is_end;
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
    virtual ~list ();
    list (list const & a);
    void swap (list b);
    bool empty () const;
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
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    reverse_iterator rbegin()
    {
        return reverse_iterator (iterator (this));
    }
    reverse_iterator rend()
    {
        return reverse_iterator (iterator (next));
    }
    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator (const_iterator (const_cast <list *> (this))); //
    }
    const_reverse_iterator rend() const
    {
        return const_reverse_iterator (const_iterator (const_cast <list *> (next))); //
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
        return const_iterator (const_cast <list *> (next)); //
    }
    const_iterator end() const
    {
        return const_iterator (const_cast <list *> (this)); //
    }
    iterator insert (const_iterator to, T const & a)
    {
        assert (to.valid);
        assert (to.check());
        assert (to.gpointer->type == this);
        list <T> * tmp = static_cast <list *> (new node <T> (a, this));
        const_iterator to_ = to;
        if (to != begin())
            to--;
        else 
            to = end();
        merge(tmp, const_cast <list *> (to_.get()));
        merge(const_cast <list *> (to.get()), tmp);
        return iterator(tmp->next);
    }
    iterator erase (const_iterator to) //invalidate
    {
        assert (to.valid);
        assert (to.check());
        assert (to.gpointer->type == this);
        assert (!(to.get()->is_end));
        list * tmp = split(const_cast <list *> (to.get()));
        delete to.get();
        return iterator(tmp);
    }
    
    iterator erase (const_iterator a, const_iterator b)
    {
        assert(a.valid);
        assert(b.valid);
        assert(a.check());
        assert(b.check());
        assert (a.gpointer->type == this);
        assert (b.gpointer->type == this);
        assert (!(a.get()->is_end));
        for (const_iterator i = a; i != b; ++i)
        {}
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
        assert(a.valid);
        assert(b.valid);
        assert(a.check());
        assert(b.check());
        assert (pos.valid);
        assert (pos.check());
        assert (a.gpointer->type == &other);
        assert (b.gpointer->type == &other);
        assert (pos.gpointer->type == this);
        if (a == b)
            return;
        for (const_iterator i = a; i != b; ++i)
        {
            //TODO in global field type
            assert (i != pos);
            i.gpointer->type = this;
            /*
            for (typename std::vector <iterator *> :: iterator j = i.get()->list_iter.begin(); j != i.get()->list_iter.end(); ++j)
                (*j)->type = this;
            for (typename std::vector <const_iterator *> :: iterator j = i.get()->list_c_iter.begin(); j != i.get()->list_c_iter.end(); ++j)
                (*j)->type = this;
            */
        }
        
        list * m = const_cast <list *> (b.get()->prev);
        list * l = split_l(const_cast <list *> (a.get()));
        list * r = split_r(const_cast <list *> (b.get()));
        const_iterator pos_ = pos;
        if (pos != begin())
            pos--;
        else 
            pos = end();
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
public:
    //std::vector <list <T>::      iterator*> list_iter; 
    //std::vector <list <T>::const_iterator*> list_c_iter; 
};

template <typename T>
void swap(list <T> & a, list <T> & b) //invalidate
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
    next(this),
    root(this),
    is_end(1)
{
    gpointer = new global <T> (root); //TODO not this  
}
/*
template <typename T> list <T>::
list (T const & a):
    prev(this),
    next(this)
{
}
*/
template <typename T> list <T>::
~list () //invalidate all
{
    gpointer->cnt--;
    gpointer->valid = 0;
    if (gpointer->cnt == 0)
        delete gpointer;
    /*
    for (typename std::vector <list <T> :: iterator *> :: iterator i = list_iter.begin(); i != list_iter.end(); ++i)
    {
        (*i)->valid = 0;
    }
    for (typename std::vector <list <T> :: const_iterator *> :: iterator i = list_c_iter.begin(); i != list_c_iter.end(); ++i)
    {
        (*i)->valid = 0;
    }
    */
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
        next(this),
        root(this),
        is_end(1)
{
    gpointer = new global <T> (this); //TODO not this
    for (const_iterator i = a.begin(); i != a.end(); ++i)
        push_back(*i);
}
template <typename T> void list <T>::
swap (list b) //invalidate
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
empty () const
{
    return (this == next && this == prev);
}
template <typename T> void list <T>::   //invalidate
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
    assert (!empty());
    return static_cast <node <T> *> (next)->val;
}
template <typename T> T & list <T>::
front()
{
    assert (!empty());
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
    erase(--end());
}
template <typename T> T const & list <T>::
back() const
{
    assert (!empty());
    return static_cast <node <T> *> (prev)->val;
}
template <typename T> T & list <T>::
back()
{
    assert (!empty());
    return static_cast <node <T> *> (prev)->val;
}
template <typename T> struct list <T>::
iterator
{
private:
    global <T> * gpointer;
    list * val;
    bool valid;
    explicit iterator(const_iterator a):
        gpointer(a.gpointer),
        val(const_cast <list *> (a.get())),
        valid(a.valid)
    {
        //std::cout << "MAKE FROM CONST_ITERATOR" << std::endl;
        if (valid)
        {
            //val->list_iter.push_back(this);
            gpointer->cnt++;
        }
    }
    bool check() const
    {
        if (!valid || gpointer == 0)
            return 0;
        return gpointer->valid;
    }
    void push_i()
    {
        if (!valid || gpointer == 0)
        {
            gpointer->cnt--;
            if (gpointer->cnt == 0)
                delete gpointer;
        }
        gpointer = val->gpointer;
        gpointer->cnt++;
    }
    void clear_i()
    {
        /*
        std::vector <iterator *> tmp;
        for (typename std::vector <iterator *> :: iterator i = val->list_iter.begin(); i != val->list_iter.end(); ++i)
            if ((*i) != this)
                tmp.push_back(*i);
        val->list_iter = tmp;
        */
        if (!valid || gpointer == 0)
            return;
        gpointer->cnt--;
        if (gpointer->cnt == 0)
            delete gpointer;
    }
public:
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    list * get() const
    {
        return val;
    }
    iterator():
        gpointer(0),
        val(0),
        valid(0)
    {   
    }
    iterator(list * a):
        val(a),
        valid(1)
    {
        gpointer = a->gpointer;
        gpointer->cnt++;
        //val->list_iter.push_back(this);
    }
    
    iterator (iterator const & a):
        gpointer(a.gpointer),
        val(a.val),
        valid(a.valid)
    {
        if (valid)
        {
            //val->list_iter.push_back(this);
            gpointer->cnt++;
        }
    }
    
    iterator & operator = (iterator const & a)
    {
        if (valid)
            clear_i();
        gpointer = a.gpointer;
        val = a.val,
        valid = a.valid;
        if (valid)
        {
            //val->list_iter.push_back(this);
            gpointer->cnt++;
        }
        return *this;
    }
    
    iterator & operator ++ ()
    {
        assert(valid);
        assert(check());
        assert(!(val->is_end));
        clear_i();
        val = val->next;
        //val->list_iter.push_back(this);
        push_i();
        return *this;
    }
    iterator & operator -- ()
    {
        assert(valid);
        assert(check());
        assert(!(val->prev->is_end));
        clear_i();
        val = val->prev;
        push_i();
        //val->list_iter.push_back(this);
        return *this;
    }
    iterator operator ++ (int)
    {
        assert(valid);
        assert(check());
        assert(!(val->is_end));
        clear_i();
        val = val->next;
        push_i();
        //val->list_iter.push_back(this);
        return iterator (val->prev);
    }
    iterator operator -- (int)
    {
        assert(valid);
        assert(check());
        assert(!(val->prev->is_end));
        clear_i();
        val = val->prev;
        push_i();
        //val->list_iter.push_back(this);
        return iterator (val->next);
    }
    T & operator * ()
    {
        assert(valid);
        assert(check());
        assert(!(val->is_end));
        return static_cast <node <T> *> (val)->val;
    }
    friend bool operator != (iterator const & b, iterator const & a)
    {
        assert(a.valid);
        assert(b.valid);
        assert(a.check());
        assert(b.check());
        return a.val != b.val;
    }
    friend bool operator == (iterator const & a, iterator const & b)
    {
        assert(a.valid);
        assert(b.valid);
        assert(a.check());
        assert(b.check());
        return a.val == b.val;
    }
    ~iterator ()
    {
        if (valid)
            clear_i();
    }
    friend struct list;
};
template <typename T> struct list <T>::
const_iterator
{
private:
    global <T> * gpointer;
    //list * type;
    list /*const*/ * val;
    bool valid;
    bool check() const
    {
        if (!valid || gpointer == 0)
            return 0;
        return gpointer->valid;
    }
    void push_i()
    {
        if (!valid || gpointer == 0)
        {
            gpointer->cnt--;
            if (gpointer->cnt == 0)
                delete gpointer;
        }
        gpointer = val->gpointer;
        gpointer->cnt++;
    }
    void clear_i()
    {
        /*
        std::vector <const_iterator *> tmp;
        for (typename std::vector <const_iterator *> :: iterator i = val->list_c_iter.begin(); i != val->list_c_iter.end(); ++i)
            if ((*i) != this)
                tmp.push_back(*i);
        val->list_c_iter = tmp;
        */
        if (!valid || gpointer == 0)
            return;
        gpointer->cnt--;
        if (gpointer->cnt == 0)
            delete gpointer;
    }
public:
    typedef const T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    list * get() const
    {
        return const_cast <list *> (val);
    }
    const_iterator(/*const*/ list * a):
        gpointer(a->gpointer),
        val(a),
        valid(1)
    {
        gpointer->cnt++;
        //val->list_c_iter.push_back(this);
    }
    const_iterator():
        gpointer(nullptr),
        val(0),
        valid(0)
    {}
    const_iterator(iterator const & a):
        gpointer(a.gpointer),
        val(a.get()),
        valid(a.valid)
    {
        if (valid)
        {
            gpointer->cnt++;
            //val->list_c_iter.push_back(this);
        }
    }
    const_iterator (const_iterator const & a):
        gpointer(a.gpointer),
        val(a.val),
        valid(a.valid)
    {
        if (valid)
        {
            gpointer->cnt++;
            //val->list_c_iter.push_back(this);
        }
    }
    const_iterator & operator = (const_iterator const & a)
    {
        if (valid)
            clear_i();
        val = a.val,
        valid = a.valid;
        gpointer = a.gpointer;
        if (valid)
        {
            //val->list_c_iter.push_back(this);
            gpointer->cnt++;
        }
        return *this;
    }
    const_iterator operator ++ ()
    {
        assert(valid);
        assert(check());
        assert(!(val->is_end));
        clear_i();
        val = static_cast <list <T> *> (val->next);
        push_i();
        //val->list_c_iter.push_back(this);
        return const_iterator (val);
    }
    const_iterator operator -- ()
    {
        assert(valid);
        assert(check());
        assert(!(val->prev->is_end));
        clear_i();
        val = val->prev;
        push_i();
        //val->list_c_iter.push_back(this);
        return const_iterator (val);
    }
    const_iterator operator ++ (int)
    {
        assert(valid);
        assert(check());
        assert(!(val->is_end));
        clear_i();
        val = val->next;
        push_i();
        //val->list_c_iter.push_back(this);
        return const_iterator (val->prev);
    }
    const_iterator operator -- (int)
    {
        assert(valid);
        assert(check());
        assert(!(val->prev->is_end));
        clear_i();
        val = val->prev;
        push_i();
        //val->list_c_iter.push_back(this);
        return const_iterator (val->next);
    }
    friend bool operator != (const_iterator const & a, const_iterator const & b)
    {
        assert(a.valid);
        assert(b.valid);
        assert(a.check());
        assert(b.check());
        return a.val != b.val;
    }
    friend bool operator == (const_iterator const & a, const_iterator const & b)
    {
        assert(a.valid);
        assert(b.valid);
        assert(a.check());
        assert(b.check());
        return a.val == b.val;
    }
    T const & operator * ()
    {
        assert(valid);
        assert(check());
        assert(!(val->is_end));
        return static_cast <node <T> const *> (val)->val;
    }
    friend struct list;
    ~const_iterator ()
    {
        if (valid)
            clear_i();
    }
};
template <typename T> 
struct node : list <T>
{
public:
    T val;
    node (T const & a, list <T> * root_):
        list <T> :: list(),
        val(a)
    {
        list <T> :: is_end = 0;
        list <T> :: gpointer->type = root_;
    }
};
