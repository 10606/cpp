#ifndef LINKED_PTR
#define LINKED_PTR
#include <utility>
#include <cstdlib>

template <typename T>
struct linked_ptr
{
private:
    T * data;
    linked_ptr <T> const mutable * left, * rigth;

public:
    linked_ptr ():
        data(nullptr),
        left(this),
        rigth(this)
    {}

    linked_ptr (T * a):
        linked_ptr() 
    {
        data = a;
    }

    
    linked_ptr (linked_ptr const & a):
        data(a.data),
        left(&a),
        rigth(a.rigth)
    {
        left -> rigth = this;
        rigth -> left = this;
    }

    void swap (linked_ptr & a) noexcept
    {
        if (data == a.data)
            return;
        std::swap(data, a.data);
        if (left == this && &a == a.left)
        {}
        else if (left == this)
        {
            left = a.left;
            rigth = a.rigth;
            left->rigth = this;
            rigth->left = this;
            a.left = &a;
            a.rigth = &a;
        }
        else if (a.left == &a)
        {
            a.left = left;
            a.rigth = rigth;
            a.left->rigth = &a;
            a.rigth->left = &a;
            left = this;
            rigth = this;
        }
        else
        {
            std::swap(left, a.left);
            std::swap(rigth, a.rigth);
            a.left->rigth = &a;
            a.rigth->left = &a;
            left->rigth = this;
            rigth->left = this;

        }

    }

    linked_ptr & operator = (linked_ptr a) noexcept
    {
        swap(a);
        return *this;
    }

    ~linked_ptr ()
    {   
        if (left == this)
        {
            if (data != nullptr)
                delete data;
            return;
        }
        left->rigth = rigth;
        rigth->left = left;

        
    }

    T * get () const noexcept
    {
        return data;
    }

    template <typename ... Arg>
    static linked_ptr <T> make(Arg && ... args)
    {
        return linked_ptr <T> (new T(std::forward <Arg> (args)...));
    }

    friend bool operator == (linked_ptr const & a, linked_ptr const & b) noexcept 
    { 
         return a.data == b.data; 
    }
    friend bool operator != (linked_ptr const & a, linked_ptr const & b) noexcept 
    { 
        return a.data != b.data; 
    }

};
#endif
