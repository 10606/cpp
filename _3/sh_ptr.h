
#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <memory>
class sh_ptr
{
    uint32_t * val;
    size_t * cnt;
public:
    sh_ptr():
        val(0),
        cnt(0)
    {}
    sh_ptr(sh_ptr const & a):
        val(a.val),
        cnt(a.cnt)
    {
        if (cnt != 0)
            ++(*cnt);
    }
    ~sh_ptr()
    {
        if (cnt == 0)
            return;
        --(*cnt);
        if (*cnt == 0)
        {
            delete cnt;
            delete [] val;
        }
    }
    sh_ptr(uint32_t * val_):
        val(val_),
        cnt(new size_t)
    {
        *cnt = 1;
    }
    uint32_t * get()
    {
        return val;
    }
    void reset(uint32_t * val_)
    {
        if (cnt != 0)
            --(*cnt);
        if (cnt != 0 && *cnt == 0)
        {
            delete cnt;
            delete [] val;
        }
        val = val_;
        cnt = new size_t;
        *cnt = 1;
    }
    void reset(sh_ptr const & a)
    {
        if (cnt != 0)
            --(*cnt);
        if (cnt != 0 && *cnt == 0)
        {
            delete cnt;
            delete [] val;
        }
        val = a.val;
        cnt = a.cnt;
        if (cnt != 0)
            ++(*cnt);
    }
    void set(uint32_t * val_)
    {
        val = val_;
        cnt = new size_t;
        *cnt = 1;
    }
    void set(sh_ptr const & a)
    {
        val = a.val;
        cnt = a.cnt;
        if (cnt != 0)
            ++(*cnt);
    }
    bool unique()
    {
        return (cnt == 0 || *cnt == 0);
    }
    sh_ptr & operator = (sh_ptr const & a)
    {
        reset(a);
        return *this;
    }
};
