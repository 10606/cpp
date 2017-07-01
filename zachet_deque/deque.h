#pragma once
#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <cstdio>
#include <memory>
template <typename T>
struct deque
{
public:
    struct iterator;
    struct const_iterator;
private:
    size_t start;
    size_t siz;
    size_t capacity;
    T * data;
    void realloc();
    void swap(deque);
    size_t down(size_t & i)
    {
        if (i == start)
            i = (start + siz - 1) % capacity;
        else
            i = (i + capacity - 1) % capacity;
        return i;
    }
    size_t up(size_t & i)
    {
        if (i == (start + siz - 1) % capacity)
            i = start;
        else
            i = (i + 1) % capacity;
        return i;
    }
    bool go_l(const_iterator);
    void swap_d (size_t, size_t); 
    void swap_f (size_t, size_t); 
public:
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    deque();
    ~deque();
    deque(deque const &);
    deque & operator = (deque const &);
    bool empty() const;
    size_t size() const;
    void clear();
    void push_back(T const &);
    void push_front(T const &);
    void pop_back();
    void pop_front();
    T const & back() const;
    T const & front() const;
    T & back();
    T & front();
    iterator insert (const_iterator pos, T const & a); 
    iterator erase (const_iterator pos); 
    T & operator [] (size_t i); 
    T const & operator [] (size_t i) const; 
    
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    
    template <typename R>
    friend void swap(deque <R> & a, deque <R> & b);
};
/*
template <typename T>
std::ostream & operator << (std::ostream & s, deque <T> a)
{
    for (typename deque<T>::const_iterator i = a.begin(); i != a.end(); ++i)
        s << *i << ", ";
    return s;
}
*/
#include "iterators.h"

template <typename T>
bool deque<T>::go_l(const_iterator a)
{
    std::ptrdiff_t l = a - begin();
    std::ptrdiff_t r = static_cast <const_iterator> (end()) - a;
    return l < r;
}

template <typename T>
void deque<T>::swap_f (size_t pos_b, size_t pos_e) //FIXME
{
    pos_e = (pos_e + capacity - 1) % capacity;
    for (size_t i = pos_b; i != pos_e;)
    {
        size_t j = i;
        up(i);
        std::swap(data[i], data[j]);
    }
}

template <typename T>
void deque<T>::swap_d (size_t pos_b, size_t pos_e) //FIXME
{
    for (size_t i = pos_e; i != pos_b;)
    {
        size_t j = i;
        down(i);
        std::swap(data[i], data[j]);
    }
}

template <typename T>
void deque<T>::realloc()
{
    if (capacity == 0)
    {
        data = static_cast <T*> (operator new (sizeof(T)));
        capacity = 1;
        return;
    }
    T * t_data = static_cast <T*> (operator new (capacity * 2 * sizeof(T)));
    for (size_t i = 0; i != siz; ++i)
    {
        std::swap(t_data[i], data[(start + i) % capacity]);
    }
    std::swap(data, t_data);
    operator delete (t_data);
    capacity *= 2;
    start = 0;
};

template <typename T>
deque<T>::deque():
    start(0),
    siz(0),
    capacity(0),
    data(0)
{
}

template <typename T>
deque<T>::~deque()
{
    if (!empty())
        operator delete (data);
}

template <typename T>
deque<T>::deque(deque const & a):
    start(0),
    siz(0),
    capacity(0),
    data(0)
{
    for (const_iterator i = a.begin(); i != a.end(); ++i)
        push_back((*i));
}

template <typename T>
void deque<T>::swap (deque <T> a)
{
    std::swap(data, a.data);
    std::swap(siz, a.siz);
    std::swap(capacity, a.capacity);
    std::swap(start, a.start);
}

template <typename T>
deque<T> & deque<T>::operator = (deque const & a)
{
    swap(a);
    return *this;
}

template <typename T>
bool deque<T>::empty() const
{
    return (siz == 0);
}

template <typename T>
size_t deque<T>::size() const
{
    return siz;
}

template <typename T>
void deque<T>::clear()
{
    if (!empty())
        operator delete (data);
    siz = 0;
    start = 0;
    capacity = 0;
    data = 0;
}

template <typename T>
void deque<T>::push_back(T const & a)
{
    if (siz == capacity)
        realloc();
    data[(start + siz) % capacity] = a;
    siz++;
}

template <typename T>
void deque<T>::push_front(T const & a)
{
    if (siz == capacity)
        realloc();
    size_t new_start = (start + capacity - 1) % capacity;
    data[new_start] = a;
    siz++;
    start = new_start;
}

template <typename T>
void deque<T>::pop_back()
{
    data[(siz + start) % capacity].~T();
    siz--;
}

template <typename T>
void deque<T>::pop_front()
{
    data[(start) % capacity].~T();
    start = (start + 1) % capacity;
    siz--;
}

template <typename T>
T const & deque<T>::back() const
{
    return data[(start + siz - 1) % capacity];
}

template <typename T>
T const & deque<T>::front() const
{
    return data[start];
}

template <typename T>
T & deque<T>::back()
{
    return data[(start + siz - 1) % capacity];
}

template <typename T>
T & deque<T>::front()
{
    return data[start];
}

template <typename T>
typename deque<T>::iterator deque<T>::insert (const_iterator pos, T const & a) //TODO
{
    std::ptrdiff_t ind;
    if (!go_l(pos))
    {
        ind = pos - begin();
        push_back(a);
        swap_d((ind + start) % capacity, (start + siz + capacity - 1) % capacity);
        return deque<T>::iterator(data, (ind + start) % capacity, capacity);
    }
    else   
    {
        ind = pos - begin();
        push_front(a);
        ind = (ind + 1) % capacity;
        swap_f(start, (ind + start) % capacity);
        //return deque<T>::iterator(data, (ind + start) % capacity, capacity);
        return deque<T>::iterator(data, (ind + start - 1 + capacity) % capacity, capacity);
    }
}

template <typename T>
typename deque<T>::iterator deque<T>::erase (const_iterator pos) //TODO
{
    std::ptrdiff_t ind;
    if (!go_l(pos))
    {
        ind = pos - begin();
        swap_f((ind + start) % capacity, (start + siz + capacity) % capacity);
        pop_back();
        return deque<T>::iterator(data, (ind + start) % capacity, capacity);
    }
    else   
    {
        ind = pos - begin();
        swap_d(start, (start + ind) % capacity);
        pop_front();
        return deque<T>::iterator(data, (ind + start - 1 - capacity) % capacity, capacity);
    }
}

template <typename T>
T & deque<T>::operator [] (size_t i)
{
    return data[(start + i) % capacity];
}

template <typename T>
T const & deque<T>::operator [] (size_t i) const
{
    return data[(start + i) % capacity];
}

template <typename T>
typename deque<T>::iterator deque<T>::begin()
{
    return iterator (data, start, capacity);
}

template <typename T>
typename deque<T>::iterator deque<T>::end()
{
    return iterator (data, start + siz, capacity);
}

template <typename T>
typename deque<T>::const_iterator deque<T>::begin() const
{
    return const_iterator (data, start, capacity);
}

template <typename T>
typename deque<T>::const_iterator deque<T>::end() const
{
    return const_iterator (data, start + siz, capacity);
}

template <typename T>
typename deque<T>::reverse_iterator deque<T>::rbegin()
{
    return reverse_iterator(end());
}

template <typename T>
typename deque<T>::reverse_iterator deque<T>::rend()
{
    return reverse_iterator(begin());
}

template <typename T>
typename deque<T>::const_reverse_iterator deque<T>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <typename T>
typename deque<T>::const_reverse_iterator deque<T>::rend() const
{
    return const_reverse_iterator(begin());
}

template <typename R>
void swap(deque <R> & a, deque <R> & b)
{
    std::swap(b.data, a.data);
    std::swap(b.siz, a.siz);
    std::swap(b.capacity, a.capacity);
    std::swap(b.start, a.start);
}

//comare iterator const_iterator
//*reverse_iterator 
