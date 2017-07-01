#pragma once
template <typename T> //TODO
struct deque<T>::iterator
{
private:
    T * data;
    size_t pos;
    size_t capacity;
public:
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    
    template <typename G>
    iterator (typename deque <G>::const_iterator a, typename std::enable_if <std::is_same <T, const G>::value>::type* = nullptr)
    {
        data = a.data;
        pos = a.pos;
        capacity = a.capacity;
    }
                         
    iterator & operator = (iterator const & a)
    {
        data = a.data;
        pos = a.pos;
        capacity = a.capacity;
        return *this;
    }
    iterator (iterator const & a):
        data(a.data),
        pos(a.pos),
        capacity(a.capacity)
    {}
    iterator (T * data_, size_t pos_, size_t capacity_):
        data(data_),
        pos(pos_),
        capacity(capacity_)
    {}
    ~iterator ()
    {}
    iterator ()
    {}
    /*
    template <typename D>
    std::ptrdiff_t operator - (typename deque<D>::iterator const & a) const
    {
        return pos - a.pos;
    }
    template <typename D>
    bool operator < (typename deque<D>::iterator const & a) const
    {
        return ((*this) - a) < 0;
    }
    template <typename D>
    bool operator <= (typename deque<D>::iterator const & a) const
    {
        return ((*this) - a) <= 0;
    }
    template <typename D>
    bool operator > (typename deque<D>::iterator const & a) const
    {
        return ((*this) - a) > 0;
    }
    template <typename D>
    bool operator >= (typename deque<D>::iterator const & a) const
    {
        return ((*this) - a) >= 0;
    }
    template <typename D>
    bool operator != (typename deque<D>::iterator const & a) const
    {
        return (data != a.data) || (pos != a.pos);
    }
    template <typename D>
    bool operator == (typename deque<D>::iterator const & a) const
    {
        return (data == a.data) && (pos == a.pos);
    }
    */
    T & operator *() const
    {
        return *(data + (pos  % capacity));
    }
    T & operator [] (std::ptrdiff_t a) const
    {
        return *(*this + a);
    }
    iterator & operator -= (std::ptrdiff_t a)
    {
        pos = (pos - a);
        return *this;
    }
    iterator & operator += (std::ptrdiff_t a)
    {
        pos = (pos + a);
        return *this;
    }
    iterator operator - (std::ptrdiff_t a)
    {
        size_t t_pos = (pos - a);
        return iterator(data, t_pos, capacity);
    }
    iterator operator + (std::ptrdiff_t a)
    {
        size_t t_pos = (pos + a);
        return iterator(data, t_pos, capacity);
    }
    iterator & operator -- ()
    {
        *this = *this - 1;
        return *this;
    }
    iterator & operator ++ ()
    {
        *this = *this + 1;
        return *this;
    }
    iterator operator -- (int)
    {
        iterator tmp = *this;
        *this = *this - 1;
        return tmp;;
    }
    iterator operator ++ (int)
    {
        iterator tmp = *this;
        *this = *this + 1;
        return tmp;;
    }
    
    
    template <typename D>
    friend typename deque<D>::iterator operator + (std::ptrdiff_t a, typename deque<D>::iterator b);
    friend struct const_iterator;
    
    friend std::ptrdiff_t operator - (typename deque<T>::iterator a, typename deque<T>::iterator b)
    {
        return a.pos - b.pos;
    }
    friend bool operator < (typename deque<T>::iterator a, typename deque<T>::iterator b)
    {
        return (a - b) < 0;
    }
    friend bool operator <= (typename deque<T>::iterator a, typename deque<T>::iterator b)
    {
        return (a - b) <= 0;
    }
    friend bool operator > (typename deque<T>::iterator a, typename deque<T>::iterator b)
    {
        return (a - b) > 0;
    }
    friend bool operator >= (typename deque<T>::iterator a, typename deque<T>::iterator b)
    {
        return (a - b) >= 0;
    }
    friend bool operator != (typename deque<T>::iterator a, typename deque<T>::iterator b)
    {
        return (b.data != a.data) || (b.pos != a.pos);
    }
    friend bool operator == (typename deque<T>::iterator a, typename deque<T>::iterator b)
    {
        return (b.data == a.data) && (b.pos == a.pos);
    }
};


template <typename T>
typename deque<T>::iterator operator + (std::ptrdiff_t a, typename deque<T>::iterator b)
{
    size_t t_pos = (b.pos + a);
    return iterator(b.data, t_pos, b.capacity);
}

template <typename T> //TODO
struct deque<T>::const_iterator
{
private:
    T const * data;
    size_t pos;
    size_t capacity;
public:
    typedef T const value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T const * pointer;
    typedef T const & reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    
    /*
    template <typename G>
    const_iterator (typename deque <G>::iterator a, typename std::enable_if <std::is_same <const T, G>::value>::type* = nullptr)
                         
    {
        data = a.data;
        pos = a.pos;
        capacity = a.capacity;
    }
    */
    const_iterator (iterator const & a):
        data(a.data),
        pos(a.pos),
        capacity(a.capacity)
    {}
    
    const_iterator & operator = (const_iterator const & a)
    {
        data = a.data;
        pos = a.pos;
        capacity = a.capacity;
        return *this;
    }
    const_iterator (const_iterator const & a):
        data(a.data),
        pos(a.pos),
        capacity(a.capacity)
    {}
    const_iterator (T const * data_, size_t pos_, size_t capacity_):
        data(data_),
        pos(pos_),
        capacity(capacity_)
    {}
    const_iterator ()
    {}
    T const & operator * () const 
    {
        return data[pos % capacity];
    }
    T const & operator [] (std::ptrdiff_t a) const
    {
        return *(*this + a);
    }
    const_iterator & operator -= (std::ptrdiff_t a)
    {
        pos = (pos - a);
        return *this;
    }
    const_iterator & operator += (std::ptrdiff_t a)
    {
        pos = (pos + a);
        return *this;
    }
    const_iterator operator - (std::ptrdiff_t a)
    {
        size_t t_pos = (pos - a);
        return const_iterator(data, t_pos, capacity);
    }
    const_iterator operator + (std::ptrdiff_t a)
    {
        size_t t_pos = (pos + a);
        return const_iterator(data, t_pos, capacity);
    }
    const_iterator & operator -- ()
    {
        *this = *this - 1;
        return *this;
    }
    const_iterator & operator ++ ()
    {
        *this = *this + 1;
        return *this;
    }
    const_iterator operator -- (int)
    {
        const_iterator tmp = *this;
        *this = *this - 1;
        return tmp;;
    }
    const_iterator operator ++ (int)
    {
        const_iterator tmp = *this;
        *this = *this + 1;
        return tmp;;
    }
    /*
    template <typename D>
    std::ptrdiff_t operator - (typename deque<D>::const_iterator const & a) const
    {
        return pos - a.pos;
    }
    template <typename D>
    bool operator < (typename deque<D>::const_iterator const & a) const
    {
        return ((*this) - a) < 0;
    }
    template <typename D>
    bool operator <= (typename deque<D>::const_iterator const & a) const
    {
        return ((*this) - a) <= 0;
    }
    template <typename D>
    bool operator > (typename deque<D>::const_iterator const & a) const
    {
        return ((*this) - a) > 0;
    }
    template <typename D>
    bool operator >= (typename deque<D>::const_iterator const & a) const
    {
        return ((*this) - a) >= 0;
    }
    template <typename D>
    bool operator != (typename deque<D>::const_iterator const & a) const
    {
        return (data != a.data) || (pos != a.pos);
    }
    template <typename D>
    bool operator == (typename deque<D>::const_iterator const & a) const
    {
        return (data == a.data) && (pos == a.pos);
    }
    */
    
    friend std::ptrdiff_t operator - (typename deque<T>::const_iterator a, typename deque<T>::const_iterator b)
    {
        return a.pos - b.pos;
    }
    friend bool operator < (typename deque<T>::const_iterator a, typename deque<T>::const_iterator b)
    {
        return (a - b) < 0;
    }
    friend bool operator <= (typename deque<T>::const_iterator a, typename deque<T>::const_iterator b)
    {
        return (a - b) <= 0;
    }
    friend bool operator > (typename deque<T>::const_iterator a, typename deque<T>::const_iterator b)
    {
        return (a - b) > 0;
    }
    friend bool operator >= (typename deque<T>::const_iterator a, typename deque<T>::const_iterator b)
    {
        return (a - b) >= 0;
    }
    friend bool operator != (typename deque<T>::const_iterator a, typename deque<T>::const_iterator b)
    {
        return (b.data != a.data) || (b.pos != a.pos);
    }
    friend bool operator == (typename deque<T>::const_iterator a, typename deque<T>::const_iterator b)
    {
        return (b.data == a.data) && (b.pos == a.pos);
    }
};


template <typename T>
typename deque<T>::const_iterator operator + (std::ptrdiff_t a, typename deque<T>::const_iterator b)
{
    size_t t_pos = (b.pos + a);
    return const_iterator(b.data, t_pos, b.capacity);
}

