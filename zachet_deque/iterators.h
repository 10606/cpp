
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
    std::ptrdiff_t operator - (deque<T>::iterator a)
    {
        return pos - a.pos;
    }
    bool operator < (deque<T>::iterator a)
    {
        return ((*this) - a) < 0;
    }
    bool operator <= (deque<T>::iterator a)
    {
        return ((*this) - a) <= 0;
    }
    bool operator > (deque<T>::iterator a)
    {
        return ((*this) - a) > 0;
    }
    bool operator >= (deque<T>::iterator a)
    {
        return ((*this) - a) >= 0;
    }
    bool operator != (deque<T>::iterator a)
    {
        return (data != a.data) || (pos != a.pos);
    }
    bool operator == (deque<T>::iterator a)
    {
        return (data == a.data) && (pos == a.pos);
    }
    T & operator *()
    {
        return *(data + (pos  % capacity));
    }
    T & operator [] (std::ptrdiff_t a)
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
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
                         
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
    std::ptrdiff_t operator - (deque<T>::const_iterator a)
    {
        return pos - a.pos;
    }
    bool operator < (deque<T>::const_iterator a)
    {
        return ((*this) - a) < 0;
    }
    bool operator <= (deque<T>::const_iterator a)
    {
        return ((*this) - a) <= 0;
    }
    bool operator > (deque<T>::const_iterator a)
    {
        return ((*this) - a) > 0;
    }
    bool operator >= (deque<T>::const_iterator a)
    {
        return ((*this) - a) >= 0;
    }
    bool operator != (deque<T>::const_iterator a)
    {
        return (data != a.data) || (pos != a.pos);
    }
    bool operator == (deque<T>::const_iterator a)
    {
        return (data == a.data) && (pos == a.pos);
    }
    T const & operator *()
    {
        return *(data + (pos  % capacity));
    }
    T const & operator [] (std::ptrdiff_t a)
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
    template <typename D>
    friend typename deque<D>::const_iterator operator + (std::ptrdiff_t a, typename deque<D>::const_iterator b);
};

template <typename T>
typename deque<T>::const_iterator operator + (std::ptrdiff_t a, typename deque<T>::const_iterator b)
{
    size_t t_pos = (b.pos + a);
    return const_iterator(b.data, t_pos, b.capacity);
}

