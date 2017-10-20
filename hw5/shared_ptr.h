#ifndef SHARED_PTR
#define SHARED_PTR
#include <utility>
#include <cstdlib>

template <typename T>
struct shared_ptr
{
private:
    struct pack
    {
        T data;
        size_t cnt;

        template <typename ... Arg>
        pack (Arg ... args):
            data (std::forward <Arg> (args)...),
            cnt(1)
        {}
    };
    pack * ptr;

public:
    shared_ptr ():
        ptr(nullptr)
    {}
    
    shared_ptr (shared_ptr const & a):
        ptr(a.ptr)
    {
        if (ptr != nullptr)
            ptr->cnt++;
    }

    void swap (shared_ptr & a) noexcept
    {
        std::swap(a.ptr, ptr);
    }

    shared_ptr & operator = (shared_ptr a) noexcept
    {
        swap(a);
        return *this;
    }

    ~shared_ptr ()
    {
        if (ptr == nullptr)
            return;
        ptr->cnt--;
        if (ptr->cnt != 0)
            return;
        delete ptr;
    }

    T * get () const noexcept
    {
        if (ptr == nullptr)
            return nullptr;
        return &(ptr->data);
    }

    template <typename ... Arg>
    static shared_ptr <T> make(Arg && ... args)
    {
        shared_ptr<T> make;
        typename shared_ptr<T>::pack * tmp = new typename shared_ptr<T>::pack(std::forward <Arg> (args)...);
        make.ptr = tmp;
        return make;
    }

    friend bool operator == (shared_ptr const & a, shared_ptr const & b) noexcept 
    { 
         return a.ptr == b.ptr; 
    }
    friend bool operator != (shared_ptr const & a, shared_ptr const & b) noexcept 
    { 
        return a.ptr != b.ptr; 
    }

};

#endif
