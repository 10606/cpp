#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
class big_int 
{
    static const uint32_t a00_ = 0;
    static const uint32_t a11_ = 0xffffffff;
    
    struct small_obj
    {
        uint32_t rigth, left;
        small_obj(int32_t a):
            rigth(a),
            left((a > 0)? a00_ : a11_)
        {}
        small_obj(std::pair<int32_t, uint32_t> a):
            rigth(a.first),
            left(a.second)
        {}
        small_obj():
            rigth(0),
            left(0)
        {}
    };
    
    struct big_obj
    {
        uint32_t * data;
        size_t capacity;
        big_obj():
            data(nullptr), 
            capacity(0)
        {}
        big_obj(size_t capacity_): 
            capacity(capacity_)
        {
            data = static_cast<uint32_t *>(operator new (capacity));
        }
    };
    
    size_t size;
    
    union soo
    {
        small_obj small;
        big_obj big;
        soo()
        {}
        ~soo()
        {}
        soo(int32_t a):
            small(a)
        {}
        soo(std::pair<int32_t, uint32_t> a):
            small(a)
        {}
    };

    soo data;

    big_int ():
        size(0),
        data()
    {}
    
    ~big_int()
    {
        if (size > 2)
            operator delete (data.big.data);
    }
    
    big_int (std::pair <uint32_t, uint32_t> a):
        size(2),
        data(a)
    {}
    
    big_int (big_int const & a)
    {
        if (size > 2)
            operator delete (data.big.data);
        size = a.size;
        if (size <= 2)
        {
            data = a.data;
        }
        else
        {
            data.big = big_obj(a.data.big.capacity);
            for (size_t i = 0; i != size; ++i)
            {
                data.big.data[i] = a.data.big.data[i];
            }
        }
    }

    explicit big_int (int32_t val):
        size(2),
        data(val)
    {}
    
    static inline std::pair<bool, uint32_t> const add(uint32_t const & a, uint32_t const & b, bool cf_ = 0) 
    {
        uint32_t pres = a + cf_;
        uint32_t res = pres + b;
        bool cf = (pres < a) || (res < pres) || (res < a);
        return std::make_pair(cf, res); 
    }

    static inline std::pair<uint32_t, uint32_t> const mul(uint32_t const & a, uint32_t const & b, uint32_t cf_ = 0)   // a * b + cf_
    {
        uint64_t res = (a * b) + cf_;
        return std::make_pair((res >> 32), res);
    }

    static const uint32_t c11_ = 0xc0000000;
    static const uint32_t c01_ = 0x40000000;
    static const uint32_t c1_ = 0x80000000;
   
    //ADD
    static inline bool check_resize_ss(big_int const & a, big_int const & b)
    {
        if (b.data.small.left < c11_ && b.data.small.left >= c01_)
            return 0;
        if (a.data.small.left < c11_ && a.data.small.left >= c01_)
            return 0;
        return 1;
    }
    
    static inline bool check_resize_sb(big_int const & a, big_int const & b) 
    {
        if (a.size > 2 && a.data.big.data[b.size - 1] < c11_ && a.data.big.data[b.size - 1] >= c01_)
            return 0;
        if (b.size > 2 && b.data.big.data[b.size - 1] < c11_ && b.data.big.data[b.size - 1] >= c01_)
            return 0;
        return 1;
    }
    
    static inline bool check_resize_bb(big_int const & a, big_int const & b)
    {
        if (b.data.big.data[b.size - 1] < c11_ && b.data.big.data[b.size - 1] >= c01_)
            return 0;
        if (a.data.big.data[b.size - 1] < c11_ && a.data.big.data[b.size - 1] >= c01_)
            return 0;
        return 1;
    }
    //TODO
    //FIXME check on big 0000111100001111 is small? NO!!
    static inline big_int const add_bb (big_int const & a, big_int const & b)
    {
        bool need_resize = check_resize_bb(a, b);
        big_int res;
        size_t max_size = std::max(a.size, b.size) + need_resize;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        bool cf = 0;
        size_t i;
        bool big0 = 0;
        bool big1 = 0;
        std::pair <bool, uint32_t> m = add(a.data.big.data[0], b.data.big.data[0], cf);
        std::pair <bool, uint32_t> l = add(a.data.big.data[1], b.data.big.data[1], cf);
        std::pair <bool, uint32_t> r;
        res.data.big.data[0] = r.second;
        cf = r.first;
    
        r = add(a.data.big.data[0], b.data.big.data[0], cf);
        res.data.big.data[1] = r.second;
        big0 |= (r.second & c1_);
        big1 |= !(r.second & c1_);
        cf = r.first;
        
        for (i = 0; i != std::min(a.size, b.size); ++i)
        {
            r = add(a.data.big.data[i], b.data.big.data[i], cf);
            res.data.big.data[i] = r.second;
            cf = r.first;
            big1 |= (r.second != a11_);
            big0 |= (r.second != a00_);
        }
        uint32_t cb = (b.data.big.data[b.size - 1] & c1_)? a11_ : a00_;
        uint32_t ca = (a.data.big.data[a.size - 1] & c1_)? a11_ : a00_;
        if (a.size < b.size)
        {
            for (; i != max_size - need_resize; ++i)
            {
                r = add(a.data.big.data[i], cb, cf);
                res.data.big.data[i] = r.second;
                cf = r.first;
                big1 |= (r.second != a11_);
                big0 |= (r.second != a00_);
            }
        }
        else
        {
            for (; i != max_size - need_resize; ++i)
            {
                r = add(b.data.big.data[i], ca, cf);
                res.data.big.data[i] = r.second;
                cf = r.first;
                big1 |= (r.second != a11_);
                big0 |= (r.second != a00_);
            }
        }
        if (need_resize)
        {
            r = add(ca, cb, cf);
            res.data.big.data[max_size - 1] = r.second;
            big1 |= (r.second != a11_);
            big0 |= (r.second != a00_);
        }
        if (!(big0 && big1)) 
        {
            operator delete (res.data.big.data);
            res.size = 2;
            res.data.small.left = l.second;
            res.data.small.rigth = m.second;
        }
        return res;
    }
    
    static inline big_int const add_ss (big_int const & a, big_int const & b)
    {
        bool need_resize = check_resize_ss(a, b);
        if (need_resize)
        {
            big_int res;
            size_t max_size = 3;
            res.size = max_size;
            res.data.big = big_obj(max_size);
            uint32_t cb = (b.data.small.left & c1_)? a11_ : a00_;
            uint32_t ca = (a.data.small.left & c1_)? a11_ : a00_;
            std::pair <bool, uint32_t> r = add(a.data.small.rigth, b.data.small.rigth);
            std::pair <bool, uint32_t> m = add(a.data.small.left, b.data.small.left, r.first);
            std::pair <bool, uint32_t> l = add(ca, cb, m.first);
            res.data.big.data[0] = r.second;
            res.data.big.data[1] = m.second;
            res.data.big.data[2] = l.second;
            return res;
        }
        else
        {
            std::pair <bool, uint32_t> r = add(a.data.small.rigth, b.data.small.rigth);
            std::pair <bool, uint32_t> l = add(a.data.small.left, b.data.small.left, r.first);
            return big_int(std::make_pair(l.second, r.second));
        }
    }
    
    static inline big_int const add_bs (big_int const & a, big_int const & b) //a - BIG, b - SMALL
    {
        bool need_resize = check_resize_ss(a, b);
        big_int res;
        size_t max_size = a.size + need_resize;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        uint32_t cb = (b.data.small.left & c1_)? a11_ : a00_;
        uint32_t ca = (a.data.big.data[a.size - 1] & c1_)? a11_ : a00_;
        std::pair <bool, uint32_t> r = add(a.data.big.data[0], b.data.small.rigth);
        std::pair <bool, uint32_t> l = add(a.data.big.data[1], b.data.small.left, r.first);
        std::pair <bool, uint32_t> m;
        res.data.big.data[0] = r.second;
        res.data.big.data[1] = l.second;
        bool cf = m.first;
        bool big0 = 0;
        bool big1 = 0;
        big0 |= (l.second & c1_);
        big1 |= !(l.second & c1_);
        for (size_t i = 2; i != a.size; ++i)
        {
            m = add(a.data.big.data[i], cb, cf);
            cf = m.first;
            big1 |= (m.second != a11_);
            big0 |= (m.second != a00_);
            res.data.big.data[i] = m.second;
        }
        if (need_resize)
        {
            m = add(ca, cb, cf);
            big1 |= (m.second != a11_);
            big0 |= (m.second != a00_);
            res.data.big.data[max_size - 1] = m.second;
            
        }
        if (!(big1 && big0)) 
        {
            operator delete (res.data.big.data);
            res.size = 2;
            res.data.small.left = l.second;
            res.data.small.rigth = r.second;
        }
        return res;
    }
   
    static inline big_int const add_sb (big_int const & a, big_int const & b) //a - SMALL, b - BIG
    {
        bool need_resize = check_resize_ss(b, a);
        big_int res;
        size_t max_size = b.size + need_resize;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        uint32_t ca = (a.data.small.left & c1_)? a11_ : a00_;
        uint32_t cb = (b.data.big.data[b.size - 1] & c1_)? a11_ : a00_;
        std::pair <bool, uint32_t> r = add(b.data.big.data[0], a.data.small.rigth);
        std::pair <bool, uint32_t> l = add(b.data.big.data[1], a.data.small.left, r.first);
        std::pair <bool, uint32_t> m;
        res.data.big.data[0] = r.second;
        res.data.big.data[1] = l.second;
        bool cf = m.first;
        bool big0 = 0;
        bool big1 = 0;
        big0 |= (l.second & c1_);
        big1 |= !(l.second & c1_);
        for (size_t i = 2; i != b.size; ++i)
        {
            m = add(b.data.big.data[i], ca, cf);
            cf = m.first;
            res.data.big.data[i] = m.second;
            big1 |= (m.second != a11_);
            big0 |= (m.second != a00_);
        }
        if (need_resize)
        {
            m = add(ca, cb, cf);
            res.data.big.data[max_size - 1] = m.second;
            big1 |= (m.second != a11_);
            big0 |= (m.second != a00_);
        }
        if (!(big0 && big1)) 
        {
            operator delete (res.data.big.data);
            res.size = 2;
            res.data.small.left = l.second;
            res.data.small.rigth = r.second;
        }
        return res;
    }

    static inline void set_zero(big_int & a) //a BIG
    {
        for (size_t i = 0; i != a.size; ++i)
        {
            a.data.big.data[i] = 0;
        }
    }

    static inline void add_mul(big_int & a, big_int const & b, uint32_t c, size_t pos) // a BIG shuld has correct size, b BIG  a += (b * c << pos);
    {
        uint32_t cb = (b.data.big.data[b.size - 1] & c1_)? a11_ : a00_;
        uint32_t cf = 0;
        std::pair <uint32_t, uint32_t> r;
        size_t i;
        for (i = 0; i != b.size; ++i)
        {
            r = mul(c, b.data.big.data[i], cf);
            a.data.big.data[pos + i] = r.second;
            cf = r.first;
        }
        for (; i + pos != a.size; ++i)
        {
            r = mul(c, cb, cf);
            a.data.big.data[pos + i] = r.second;
            cf = r.first;
        }
    }

    static inline big_int mul_ss(big_int const & a, big_int const & b)
    {
        
        std::pair <uint32_t, uint32_t> r = mul(a.data.small.rigth, b.data.small.rigth);
        std::pair <uint32_t, uint32_t> m1 = mul(a.data.small.left, b.data.small.rigth, r.first);
        std::pair <uint32_t, uint32_t> m2 = mul(a.data.small.rigth, b.data.small.rigth, m1.second);
        std::pair <uint32_t, uint32_t> l_ = mul(a.data.small.rigth, b.data.small.rigth, m1.first);
        std::pair <bool, uint32_t> l = add(l_.second, m2.first);
        l_.first += l.first;
        //r.second
        //m1.second
        //l.second
        //l_.first;
        if (((l.second == l_.first) && (l.second == a11_) && (m1.second & c1_)) || 
                (!(m1.second & c1_) && (l.second == l_.first) && (l.second == a00_)))
        {
            return big_int(std::make_pair(m1.second, r.second));
        }
        big_int res;
        res.size = 4;
        res.data.big = big_obj(4);
        res.data.big.data[0] = r.second;
        res.data.big.data[1] = m1.second;
        res.data.big.data[2] = l.second;
        res.data.big.data[3] = l_.first;
        return res;
    }

    static inline big_int mul_bb(big_int const & a, big_int const & b)
    {
        big_int res;
        size_t max_size = a.size + b.size;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        set_zero(res);
        for (size_t i = 0; i != b.size; ++i)
        {
            add_mul(res, a, b.data.big.data[i], i);
        }
        return res;
    }
    
    static inline big_int mul_bs(big_int const & a, big_int const & b) //a BIG b SMALL
    {
        big_int res;
        size_t max_size = a.size + b.size;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        set_zero(res);
        add_mul(res, a, b.data.small.rigth, 0);
        add_mul(res, a, b.data.small.left, 1);
        return res;
    }
    
    static inline big_int mul_sb(big_int const & a, big_int const & b) //a SMALL b BIG
    {
        big_int res;
        size_t max_size = a.size + b.size;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        set_zero(res);
        add_mul(res, b, a.data.small.rigth, 0);
        add_mul(res, b, a.data.small.left, 1);
        return res;
    }
};















































