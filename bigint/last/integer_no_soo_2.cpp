#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>

class big_int;
std::ostream & operator << (std::ostream & s, big_int const & a);
class big_int 
{
public:
    static const uint32_t a00_ = 0;
    static const uint32_t a11_ = 0xffffffff;
    static const uint32_t c11_ = 0xc0000000;
    static const uint32_t c01_ = 0x40000000;
    static const uint32_t c1_ = 0x80000000;
    
    struct small_obj
    {
        uint32_t rigth, left;
        small_obj(int32_t a):
            rigth((a > 0)? a : -a),
            left(a00_)
        {}
        small_obj(std::pair<int32_t, uint32_t> a):
            rigth(((a.first & c1_) != 0)? (a.second ^ a11_) + 1 : a.second),
            left(((a.first & c1_) != 0)? (a.first ^ a11_) + ((a.second == 0)? 1 : 0) : a.first)
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
            data = static_cast<uint32_t *>(operator new (capacity_));
        }
        //~big_obj()
        //{
        //    if (capacity != 0)
        //        operator delete (data);
        //}
    };
    
    size_t size;
    bool signed_;    
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
        signed_(0),
        data()
    {}
    
    ~big_int()
    {
        if (size > 0)
            operator delete (data.big.data);
    }
    
    big_int (std::pair <uint32_t, uint32_t> a):
        size(2),
        signed_((a.first & c1_) != 0),
        data(a)
    {}
    
    big_int (big_int const & a)
    {
        size = a.size;
        signed_ = a.signed_;
        //if (size <= 0) //TODO
        //    data = a.data;
        //else
        //{
            data.big = big_obj(a.data.big.capacity);
            for (size_t i = 0; i != size; ++i)
                data.big.data[i] = a.data.big.data[i];
        //}
    }

    void swap (big_int & a)
    {
        std::swap(size, a.size);
        std::swap(signed_, a.signed_);
        std::swap(data, a.data);
    }

    big_int & operator = (big_int a)
    {
        swap(a);
        return *this;
    }

    explicit big_int (int32_t val):
        size(2),
        signed_((val & c1_) != 0),
        data(val)
    {}
    
    static inline std::pair<bool, uint32_t> const add(uint32_t const & a, uint32_t const & b, bool cf_ = 0) 
    {
        uint32_t pres = a + cf_;
        uint32_t res = pres + b;
        bool cf = (pres < a) || (res < pres) || (res < b);
        return std::make_pair(cf, res); 
    }

    static inline std::pair<uint32_t, uint32_t> const mul(uint32_t const & a, uint32_t const & b, uint32_t cf_ = 0, uint32_t cf__ = 0)   // a * b + cf_
    {
        uint64_t res = ((uint64_t)a * (uint64_t)b) + (uint64_t)cf_ + (uint64_t)cf__;
        return std::make_pair((res >> 32), res & a11_);
    }

   
    //ADD
    static inline bool check_resize_ss(big_int const & a, big_int const & b)
    {
        if (b.data.small.left < c11_ && b.data.small.left >= c01_)
            return 1;
        if (a.data.small.left < c11_ && a.data.small.left >= c01_)
            return 1;
        return 0;
    }
    
    static inline bool check_resize_sb(big_int const & a, big_int const & b) 
    {
        if (a.size > 2 && a.data.big.data[b.size - 1] < c11_ && a.data.big.data[b.size - 1] >= c01_)
            return 1;
        if (b.size > 2 && b.data.big.data[b.size - 1] < c11_ && b.data.big.data[b.size - 1] >= c01_)
            return 1;
        return 0;
    }
    
    static inline bool check_resize_bb(big_int const & a, big_int const & b)
    {
        if (b.data.big.data[b.size - 1] < c11_ && b.data.big.data[b.size - 1] >= c01_)
            return 1;
        if (a.data.big.data[b.size - 1] < c11_ && a.data.big.data[b.size - 1] >= c01_)
            return 1;
        return 0;
    }
    //TODO
    //FIXME check on big 0000111100001111 is small? NO!!
    static inline big_int const add_bb (big_int const & a, big_int const & b, bool negate)
    {
        bool need_resize = check_resize_bb(a, b);
        big_int res;
        size_t max_size = std::max(a.size, b.size) + need_resize;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        bool cf = negate;
        uint32_t minus = (negate)? a11_ : a00_;
        size_t i;
        bool big0 = 0;
        bool big1 = 0;
        std::pair <bool, uint32_t> r;
    
        r = add(a.data.big.data[0], b.data.big.data[0], cf);
        res.data.big.data[1] = r.second;
        big0 |= (r.second & c1_);
        big1 |= !(r.second & c1_);
        cf = r.first;
        cf = negate;
        for (i = 0; i != std::min(a.size, b.size); ++i)
        {
            r = add(a.data.big.data[i], b.data.big.data[i] ^ minus, cf);
            res.data.big.data[i] = r.second;
            cf = r.first;
            big1 |= (r.second != a11_);
            big0 |= (r.second != a00_);
        }
        uint32_t cb = minus;
        uint32_t ca = a00_;
        if (a.size > b.size)
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
                r = add(b.data.big.data[i] ^ minus, ca, cf);
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
        res.signed_ = a.signed_;
        return res;
    }
    
    static inline big_int const add_ss (big_int const & a, big_int const & b, bool negate)
    {
        bool need_resize = check_resize_ss(a, b);
        if (need_resize)
        {
            big_int res;
            size_t max_size = 3;
            res.size = max_size;
            res.data.big = big_obj(max_size);
            uint32_t minus = (negate)? a11_ : a00_; 
            uint32_t cb = ((b.data.small.left & c1_)? a11_ : a00_) ^ minus;
            uint32_t ca = (a.data.small.left & c1_)? a11_ : a00_;
            std::pair <bool, uint32_t> r = add(a.data.small.rigth, b.data.small.rigth ^ minus, negate);
            std::pair <bool, uint32_t> m = add(a.data.small.left, b.data.small.left ^ minus, r.first);
            std::pair <bool, uint32_t> l = add(ca, cb, m.first);
            res.data.big.data[0] = r.second;
            res.data.big.data[1] = m.second;
            res.data.big.data[2] = l.second;
            return res;
        }
        else
        {
            uint32_t minus = (negate)? a11_ : a00_;
            std::pair <bool, uint32_t> r = add(a.data.small.rigth, b.data.small.rigth ^ minus, negate);
            std::pair <bool, uint32_t> l = add(a.data.small.left, b.data.small.left ^ minus, r.first);
            return big_int(std::make_pair(l.second, r.second));
        }
    }
    
    static inline big_int const add_bs (big_int const & a, big_int const & b, bool negate) //a - BIG, b - SMALL
    {
        bool need_resize = check_resize_ss(a, b);
        big_int res;
        size_t max_size = a.size + need_resize;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        uint32_t minus = (negate)? a11_ : a00_;
        uint32_t cb = ((b.data.small.left & c1_)? a11_ : a00_) ^ minus;
        uint32_t ca = (a.data.big.data[a.size - 1] & c1_)? a11_ : a00_;
        std::pair <bool, uint32_t> r = add(a.data.big.data[0], b.data.small.rigth ^ minus, negate);
        std::pair <bool, uint32_t> l = add(a.data.big.data[1], b.data.small.left ^ minus, r.first);
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
   
    static inline big_int const add_sb (big_int const & a, big_int const & b, bool negate) //a - SMALL, b - BIG
    {
        bool need_resize = check_resize_ss(b, a);
        big_int res;
        size_t max_size = b.size + need_resize;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        uint32_t minus = (negate)? a11_ : a00_;
        uint32_t ca = (a.data.small.left & c1_)? a11_ : a00_;
        uint32_t cb = ((b.data.big.data[b.size - 1] & c1_)? a11_ : a00_) ^ negate;
        std::pair <bool, uint32_t> r = add(b.data.big.data[0] ^ minus, a.data.small.rigth, negate);
        std::pair <bool, uint32_t> l = add(b.data.big.data[1] ^ minus, a.data.small.left, r.first);
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
            m = add(b.data.big.data[i] ^ minus, ca, cf);
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
            a.data.big.data[i] = 0;
    }

    static inline void add_mul(big_int & a, big_int const & b, uint32_t c, size_t pos) // a BIG shuld has correct size, b BIG  a += (b * c << pos);
    {
        uint32_t cb = a00_;
        uint32_t cf = 0;
        std::pair <uint32_t, uint32_t> r;
        size_t i;
        for (i = 0; (i != b.size) && (i + pos != a.size); ++i)
        {
            r = mul(c, b.data.big.data[i], cf, a.data.big.data[i + pos]);
            a.data.big.data[pos + i] = r.second;
            cf = r.first;
        }
        for (; (i + pos != a.size) && (cf); ++i)
        {
            r = mul(c, cb, cf, a.data.big.data[i + pos]);
            a.data.big.data[pos + i] = r.second;
            cf = r.first;
        }
    }

    static inline big_int mul_ss(big_int const & a, big_int const & b)
    {
        std::pair <uint32_t, uint32_t> r = mul(a.data.small.rigth, b.data.small.rigth);
        std::pair <uint32_t, uint32_t> m1 = mul(a.data.small.left, b.data.small.rigth, r.first);
        std::pair <uint32_t, uint32_t> m2 = mul(a.data.small.rigth, b.data.small.left, m1.second);
        std::pair <uint32_t, uint32_t> l_ = mul(a.data.small.left, b.data.small.left, m1.first, m2.first);
        //r.second
        //m1.second
        //l.second
        //l_.first;
        if (((l_.second == l_.first) && (l_.second == a11_) && (m1.second & c1_)) || 
                (!(m1.second & c1_) && (l_.second == l_.first) && (l_.second == a00_)))
            return big_int(std::make_pair(m2.second, r.second));
        big_int res;
        res.size = 4;
        res.data.big = big_obj(4);
        res.data.big.data[0] = r.second;
        res.data.big.data[1] = m2.second;
        res.data.big.data[2] = l_.second;
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
        size_t i;
        res.signed_ = a.signed_ ^ b.signed_;
        for (i = 0; i != b.size; ++i)
            add_mul(res, a, b.data.big.data[i], i);
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
    
    static inline big_int mul_bss(big_int const & a, uint32_t const & b)
    {
        std::cout << "mull_bss\n";
        big_int res;
        size_t max_size = a.size + 1;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        set_zero(res);
        add_mul(res, a, b, 0);
        return res;
    }
        
    static inline void mul_asign(big_int & a, uint32_t const & b)  //a BIG with owerflow
    {
        uint32_t cf = 0;
        for (size_t i = 0; i != a.size; ++i) 
        {
            std::pair <uint32_t, uint32_t> tmp = mul(b, a.data.big.data[i], cf);
            cf = tmp.first;
            a.data.big.data[i] = tmp.second;
        }
    }
    
    static inline void add_asign(big_int & a, uint32_t const & b)  //a BIG with owerflow
    {
        std::pair <bool, uint32_t> tmp = add(b, a.data.big.data[0], 0);
        bool cf = tmp.first;
        a.data.big.data[0] = tmp.second;
        for (size_t i = 1; i != a.size && cf; ++i) 
        {
            tmp = add(0, a.data.big.data[0], cf);
            cf = tmp.first;
            a.data.big.data[i] = tmp.second;
        }
    }

    static inline uint32_t div_asign(big_int & a, uint32_t b)
    {
        uint64_t divider = 0;
        for (size_t i = a.size - 1; i < a.size; --i)
        {
            divider = (divider << 32) + static_cast <uint64_t>(a.data.big.data[i]);
            a.data.big.data[i] = static_cast <uint32_t>(divider / static_cast <uint64_t>(b));
            divider %= b;
        }
        return static_cast <uint32_t> (divider);
    }
    
    static bool comp_fe(big_int const & a, big_int const & b) 
    {
        size_t min_size = std::min(a.size, b.size);
        bool fa = 1;
        bool fb = 1;
        if (a.size > b.size)
            for (size_t i = a.size - 1; i >= min_size; --i)
                fa &= (a.data.big.data[i] == 0);
        else
            for (size_t i = b.size - 1; i >= min_size; --i)
                fb &= (b.data.big.data[i] == 0);
        for (size_t i = min_size - 1; i < min_size; --i)
            if (a.data.big.data[i] != b.data.big.data[i])
                return 0;
        return (fb && fa);
    }
    
    static bool comp_fbe(big_int const & a, big_int const & b) 
    {
        size_t min_size = std::min(a.size, b.size);
        bool fa = 0;
        bool fb = 1;
        if (a.size > b.size)
            for (size_t i = a.size - 1; i >= min_size; --i)
                fa |= (a.data.big.data[i] != 0);
        else
            for (size_t i = b.size - 1; i >= min_size; --i)
                fb &= (b.data.big.data[i] == 0);
        bool f = 1;
        for (size_t i = min_size - 1; i < min_size; --i)
        {
            if (a.data.big.data[i] > b.data.big.data[i])
                break;
            f &= (a.data.big.data[i] >= b.data.big.data[i]);
        }
        return (fb && (fa || f));
    }

    static bool comp_fb(big_int const & a, big_int const & b) 
    {
        size_t min_size = std::min(a.size, b.size);
        bool fa = 0;
        bool fb = 1;
        if (a.size > b.size)
            for (size_t i = a.size - 1; i >= min_size; --i)
                fa |= (a.data.big.data[i] != 0);
        else
            for (size_t i = b.size - 1; i >= min_size; --i)
                fb &= (b.data.big.data[i] == 0);
        bool f;
        for (size_t i = min_size - 1; i < min_size; --i)
        {
            if (a.data.big.data[i] != b.data.big.data[i]) {
                f = a.data.big.data[i] > b.data.big.data[i];
                break;
            }
        }
        return (fb && (fa || f));
    }

    
    static inline big_int bin_bb(big_int const & a, big_int const & b, uint32_t (*func) (uint32_t, uint32_t)) 
    {
        big_int res;
        size_t max_size = std::max(a.size, b.size);
        res.size = max_size;
        res.data.big = big_obj(max_size);
        uint32_t ca_ = a.signed_ != 0;
        uint32_t xa_ = (a.signed_ != 0)? a11_ : a00_;
        uint32_t cb_ = b.signed_ != 0;
        uint32_t xb_ = (b.signed_ != 0)? a11_ : a00_;
        for (size_t i = 0; i != std::min(a.size, b.size); ++i)
        {
            res.data.big.data[i] = func((a.data.big.data[i] ^ xa_) + ca_, (b.data.big.data[i] ^ xb_) + cb_);
            ca_ &= a.data.big.data[i] == 0;
            cb_ &= b.data.big.data[i] == 0;
        }
        if (c1_ & res.data.big.data[res.size - 1])
        {
            res.signed_ = 1;
            bool cr_ = 1;
            for (size_t i = 0; i != res.size; ++i)
            {
                res.data.big.data[i] = (res.data.big.data[i] ^ a11_) + cr_;
                cr_ &= res.data.big.data[i] == 0;
            }
        }
        return res;
    }
    
    static inline uint32_t func_or (uint32_t a_, uint32_t b_) 
    {
        return a_ | b_;
    }

    static inline uint32_t func_and (uint32_t a_, uint32_t b_) 
    {
        return a_ & b_;
    }
    
    static inline uint32_t func_xor (uint32_t a_, uint32_t b_) 
    {
        return a_ ^ b_;
    }
    
    static inline big_int xor_bb(big_int const & a, big_int const & b) 
    {
        return bin_bb(a, b, &func_xor);
    }
    static inline big_int and_bb(big_int const & a, big_int const & b) 
    {
        return bin_bb(a, b, &func_and);
    }
    static inline big_int or_bb(big_int const & a, big_int const & b) 
    {    
        return bin_bb(a, b, (&func_or));
    }
    
    static inline std::string bi_to_string(big_int s)
    {
        std::string res = "";
        size_t max_size = s.size * 10;
        for (size_t i = 0; i != max_size; ++i)
        {
            uint32_t reminder = div_asign(s, 10);
            res.push_back(static_cast <char> (reminder + '0'));
        }
        for (size_t i = 0; i != (max_size / 2); ++i)
        {
            std::swap(res[i], res[max_size - i - 1]);
        }
        if (s.signed_)
            res = "-" + res;
        return res;
    }
    
    static inline big_int string_to_bi(std::string const & s)
    {
        big_int res;
        size_t max_size = (s.size() + 8) / 9;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        set_zero(res);
        if (s.size() == 0)
            return res;
        res.signed_ = (s[0] == '-');
        size_t i = (s[0] == '-') ? 1 : 0;
        for (; i != s.size(); ++i)
        {
            mul_asign(res, 10);
            add_asign(res, static_cast <uint32_t> (s[i] - '0'));
        }
        return res;
    }
    
    static inline big_int div_bb(big_int const & a, big_int const & b, bool type = 0)
    {
        big_int reminder(a);
        big_int res;
        big_int muller;
        size_t max_size = a.size;
        res.size = max_size;
        muller.size = max_size;
        res.data.big = big_obj(max_size);
        res.signed_ = a.signed_ ^ b.signed_;
        muller.data.big = big_obj(max_size);
        for (size_t i = res.size - 1; i < res.size; --i)
        {
            set_zero(muller);
            uint32_t l = a00_;
            uint32_t r = a11_;
            uint32_t m;
            while (r > l + 1)
            {
                set_zero(muller);
                m = ((uint64_t)r + (uint64_t)l) >> 1;
                add_mul(muller, b, m, i);
                if (comp_fb(muller, reminder))
                    r = m;
                else
                    l = m;
            }
            set_zero(muller);
            add_mul(muller, b, r, i);
            if (comp_fbe(reminder, muller))
                l = r;
            res.data.big.data[i] = l;
            set_zero(muller);
            add_mul(muller, b, l, i);
            reminder = add_bb(reminder, muller, 1);
        }
        if (!type)
            return res;
        else
            return reminder;
    }

    static inline big_int shr_bb(big_int a, uint32_t b) 
    {
        size_t max_size = a.size - (b / 32) + 1;
        size_t tail = b % 32;
        size_t shift = b / 32;
        big_int res;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        set_zero(res);
        res.signed_ = a.signed_;
        uint64_t tmp = 0;
        for (size_t i = a.size - 1; i != shift - 1; ++i)
        {
            tmp |= a.data.big.data[i];
            res.data.big.data[i - shift] = (tmp >> tail) & a11_;
            tmp = tmp << 32;
        }
        return res;
    }
    
    static inline big_int shl_bb(big_int a, uint32_t b) 
    {
        size_t max_size = a.size + (b / 32) + 1;
        size_t tail = b % 32;
        size_t shift = b / 32;
        big_int res;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        set_zero(res);
        res.signed_ = a.signed_;
        uint64_t tmp = 0;
        for (size_t i = a.size - 1; i < a.size; ++i)
        {
            tmp |= a.data.big.data[i];
            res.data.big.data[i + shift] = (tmp << tail) & a11_;
            tmp = tmp << 32;
        }
        return res;
    }

    static inline big_int dec_postfix(big_int & a)
    {
        size_t max_size = a.size;
        if (a.data.big.data[a.size - 1] & c11_)
            ++max_size;
        big_int res;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        set_zero(res);
        bool cf = 1;
        for (size_t i = 0; i != res.size && cf; ++i)
        {
            res.data.big.data[i] = a.data.big.data[i] - 1;
            cf = res.data.big.data[i] == a11_;
        }
        if (cf)
        {
            for (size_t i = 0; i != res.size && cf; ++i)
                res.data.big.data[i] = 0;
            res.data.big.data[0] = 1;
            res.signed_ = !res.signed_;
        }
        std::swap(a, res);
        return a;
    }

    static inline big_int & dec_prefix(big_int & a)
    {
        if (a.data.big.data[a.size - 1] & c11_)
        {
            big_int res;
            size_t max_size = a.size + 1;
            res.size = max_size;
            res.data.big = big_obj(max_size);
            set_zero(res);
            bool cf = 1;
            for (size_t i = 0; i != res.size && cf; ++i)
            {
                res.data.big.data[i] = a.data.big.data[i] - 1;
                cf = res.data.big.data[i] == a11_;
            }
            if (cf)
            {
                for (size_t i = 0; i != res.size && cf; ++i)
                    res.data.big.data[i] = 0;
                res.data.big.data[0] = 1;
                res.signed_ = !res.signed_;
            }
            a = res;
            return a;
        }
        else
        {
            bool cf = 1;
            for (size_t i = 0; i != a.size && cf; ++i)
            {
                --a.data.big.data[i];
                cf = a.data.big.data[i] == a11_;
            }
            if (cf)
            {
                for (size_t i = 0; i != a.size && cf; ++i)
                    a.data.big.data[i] = 0;
                a.data.big.data[0] = 1;
                a.signed_ = !a.signed_;
            }
            return a;
        }
    }
    
    static inline big_int inc_postfix(big_int & a)
    {
        size_t max_size = a.size;
        if (a.data.big.data[a.size - 1] & c11_)
            ++max_size;
        big_int res;
        res.size = max_size;
        res.data.big = big_obj(max_size);
        set_zero(res);
        bool cf = 1;
        for (size_t i = 0; i != res.size && cf; ++i)
        {
            res.data.big.data[i] = a.data.big.data[i] + 1;
            cf = res.data.big.data[i] == a00_;
        }
        std::swap(a, res);
        return a;
    }

    static inline big_int & inc_prefix(big_int & a)
    {
        if (a.data.big.data[a.size - 1] & c11_)
        {
            big_int res;
            size_t max_size = a.size + 1;
            res.size = max_size;
            res.data.big = big_obj(max_size);
            set_zero(res);
            bool cf = 1;
            for (size_t i = 0; i != res.size && cf; ++i)
            {
                res.data.big.data[i] = a.data.big.data[i] + 1;
                cf = res.data.big.data[i] == a00_;
            }
            a = res;
            return a;
        }
        else
        {
            bool cf = 1;
            for (size_t i = 0; i != a.size && cf; ++i)
            {
                ++a.data.big.data[i];
                cf = a.data.big.data[i] == a00_;
            }
            return a;
        }
    }

    friend const big_int operator ~ (big_int a);
    friend const big_int operator - (big_int a);
    friend const big_int operator + (big_int a);
    friend big_int & operator -- (big_int & a);
    friend big_int operator -- (big_int & a, int);
    friend big_int & operator ++ (big_int & a);
    friend big_int operator ++ (big_int & a, int);
    friend bool operator == (big_int a, big_int b);
    friend bool operator != (big_int a, big_int b);
    friend bool operator > (big_int a, big_int b);
    friend bool operator <= (big_int a, big_int b); 
    friend bool operator < (big_int a, big_int b);
    friend bool operator >= (big_int a, big_int b);
    
    friend big_int operator + (big_int a, big_int b);
    friend big_int operator - (big_int a, big_int b);
    friend big_int operator * (big_int a, big_int b);
    friend big_int operator / (big_int a, big_int b);
    friend big_int operator % (big_int a, big_int b);
    friend big_int operator ^ (big_int a, big_int b);
    friend big_int operator & (big_int a, big_int b);
    friend big_int operator | (big_int a, big_int b);
    friend big_int operator >> (big_int a, big_int b);
    friend big_int operator << (big_int a, big_int b);
    
};


const big_int operator + (big_int a)
{
    return a;
}
    
const big_int operator - (big_int a)
{
    a.signed_ ^= 1;
    return a;
}
    
big_int & operator -- (big_int & a)
{
    if (a.signed_)
        return big_int::dec_prefix(a);
    else
        return big_int::inc_prefix(a);
}
    
big_int operator -- (big_int & a, int)
{
    if (a.signed_)
        return big_int::dec_postfix(a);
    else
        return big_int::inc_postfix(a);
}
    
big_int & operator ++ (big_int & a)
{
    if (a.signed_)
        return big_int::inc_prefix(a);
    else
        return big_int::dec_prefix(a);
}
    
big_int operator ++ (big_int & a, int)
{
    if (a.signed_)
        return big_int::inc_postfix(a);
    else
        return big_int::dec_postfix(a);
}
    
bool operator == (big_int a, big_int b)
{
    return big_int::comp_fe(a, b);
}

bool operator != (big_int a, big_int b)
{
    return !big_int::comp_fe(a, b);
}

bool operator > (big_int a, big_int b)
{
    return big_int::comp_fb(a, b);
}

bool operator <= (big_int a, big_int b)
{
    return !big_int::comp_fb(a, b);
}

bool operator < (big_int a, big_int b)
{
    return !big_int::comp_fbe(a, b);
}

bool operator >= (big_int a, big_int b)
{
    return big_int::comp_fbe(a, b);
}

big_int operator - (big_int a, big_int b)
{
    if (a.signed_ != b.signed_)
    {
        return big_int::add_bb(a, b, 0);
    }
    else
    {
        if (big_int::comp_fb(a, b))
            return big_int::add_bb(a, b, 1);
        else
            return big_int::add_bb(b, a, 1);
    }
}
    
big_int operator + (big_int a, big_int b)
{
    if (a.signed_ == b.signed_)
    {
        return big_int::add_bb(a, b, 0);
    }
    else
    {
        if (big_int::comp_fb(a, b))
            return big_int::add_bb(a, b, 1);
        else
            return big_int::add_bb(b, a, 1);
    }
}
    
big_int operator * (big_int a, big_int b)
{
    return big_int::mul_bb(a, b);
}
    
big_int operator / (big_int a, big_int b)
{
    return big_int::div_bb(a, b, 0);
}
    
big_int operator % (big_int a, big_int b)
{
    return big_int::div_bb(a, b, 1);
}
    
big_int operator ^ (big_int a, big_int b)
{
    return big_int::xor_bb(a, b);
}
    
big_int operator | (big_int a, big_int b)
{
    return big_int::or_bb(a, b);
}
    
big_int operator & (big_int a, big_int b)
{
    return big_int::and_bb(a, b);
}

const big_int operator ~ (big_int a)
{
    big_int res(a);
    res.signed_ ^= 1;
    return res--;
}

big_int operator << (big_int a, big_int b)
{
    if (!b.signed_)
        return big_int::shl_bb(a, b.data.big.data[0]);
    else
        return big_int::shr_bb(a, b.data.big.data[0]);
}

big_int operator >> (big_int a, big_int b)
{
    if (!b.signed_)
        return big_int::shr_bb(a, b.data.big.data[0]);
    else
        return big_int::shl_bb(a, b.data.big.data[0]);
}

std::ostream & operator << (std::ostream & s, big_int const & a)
{
    s << "big_int ";
    if (a.size > 2)
    {
        for (size_t i = a.size - 1; i < a.size; --i)
        {
            s << a.data.big.data[i] << " ";
        }
    }
    else
    {
        s << a.data.small.left << " " << a.data.small.rigth << " ";
    }
    return s;
}
int main()
{
    big_int a = big_int::string_to_bi("-123456789");
    //std::cout << a.data.big.data[0] << std::endl;
    std::cout << big_int::bi_to_string(a) << std::endl;
    big_int b = big_int::string_to_bi("100000000");
    std::cout << big_int::bi_to_string(b) << std::endl;
    big_int c;
    c = big_int::add_bb(a, b, 1);
    std::cout << big_int::bi_to_string(c) << std::endl;
    big_int d;
    d = big_int::mul_bb(a, b);
    std::string ss = big_int::bi_to_string(d);
    std::cout << ss << std::endl;
    big_int f;
    f = big_int::div_bb(d, b);
    std::cout << big_int::bi_to_string(f) << std::endl;
    
    
    big_int a_ = big_int::string_to_bi("400000000");
    big_int b_ = big_int::string_to_bi("400000000");
    big_int d_ = big_int::add_bb(a_, b_, 0);
    std::cout << big_int::bi_to_string(d_) << std::endl;
    std::cout << big_int::bi_to_string(big_int::string_to_bi("10000000") + big_int::string_to_bi("1000000")) << std::endl;
    big_int a__ = big_int::string_to_bi("10000000");
    big_int b__ = big_int::string_to_bi("1000000000");
    big_int d__ = a__ + b__;
    std::cout << big_int::bi_to_string(d__) << std::endl;
    /*
    big_int a(std::make_pair(0xffffffff, 0xffffffff));
    big_int b(std::make_pair(0xffffffff, 0xffffffff));
    std::cout << a << std::endl; 
    bool f = big_int::check_resize_ss(a, b);
    a = big_int::add_ss(a, b, 0);
    std::cout << a << std::endl;
    big_int c = a;
    //std::cout << "kelelelleleeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeek" << std::endl;
    std::cout << a << std::endl;
    std::cout << c << std::endl;
    a = big_int::mul_ss(a, c);
    //a = big_int::mul_bb(a, c);
    //std::cout << a.data.small.left << "  " << a.data.small.rigth << "  ";
    std::cout << a << std::endl;
    std::cout << c << std::endl;
    std::cout << a.size << " " << f;
    */
}















































