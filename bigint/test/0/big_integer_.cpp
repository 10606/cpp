#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include "big_integer.h"
/*
class big_integer;
std::ostream & operator << (std::ostream & s, big_integer const & a);
class big_integer 
{
public:
    static const uint32_t a00_ = 0;
    static const uint32_t a11_ = 0xffffffff;
    static const uint32_t c11_ = 0xc0000000;
    static const uint32_t c01_ = 0x40000000;
    static const uint32_t c1_ = 0x80000000;
    
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
            data = new uint32_t [capacity_];
        }
        //~big_obj()
        //{
        //    if (capacity != 0)
        //        operator delete (data);
        //}
    };
    
    size_t size;
    bool signed_;    
    big_obj data;

    big_integer ():
        size(0),
        signed_(0),
        data()
    {}
    
    ~big_integer()
    {
        if (size > 0)
            delete [] data.data;
    }
    
    big_integer (big_integer const & a)
    {
        size = a.size;
        signed_ = a.signed_;
        data = big_obj(a.data.capacity);
        for (size_t i = 0; i != size; ++i)
            data.data[i] = a.data.data[i];
    }

    void swap (big_integer &);
    static void set_size(big_integer &, size_t);
    big_integer & operator = (big_integer a);

    big_integer (int32_t val):
        size(1),
        signed_((val & c1_) != 0),
        data(1)
    {
        data.data[0] = (signed_) ? (static_cast <uint32_t> (val) ^ a11_) + 1 : static_cast <uint32_t> (val);
    }
    explicit big_integer (std::string val);
    
    static inline std::pair<bool, uint32_t> const add(uint32_t const & a, uint32_t const & b, bool cf_ = 0);
    static inline std::pair<uint32_t, uint32_t> const mul(uint32_t const & a, uint32_t const & b, uint32_t cf_ = 0, uint32_t cf__ = 0);   // a * b + cf_
    
    static inline bool check_resize_bb(big_integer const & a, big_integer const & b);
    static inline big_integer const add_bb (big_integer const & a, big_integer const & b, bool negate);
    
    static inline void set_zero(big_integer & a); //a BIG
    static inline void add_mul(big_integer & a, big_integer const & b, uint32_t c, size_t pos); // a BIG shuld has correct size, b BIG  a += (b * c << pos);
    static inline big_integer mul_bb(big_integer const & a, big_integer const & b);
    
    static inline void mul_asign(big_integer & a, uint32_t const & b);  //a BIG with owerflow
    static inline void add_asign(big_integer & a, uint32_t const & b);  //a BIG with owerflow
    static inline uint32_t div_asign(big_integer & a, uint32_t b);
    
    static bool comp_fe(big_integer const & a, big_integer const & b); 
    static bool comp_fbe(big_integer const & a, big_integer const & b); 
    static bool comp_fb(big_integer const & a, big_integer const & b);
    
    static inline big_integer bin_bb(big_integer const & a, big_integer const & b, uint32_t (*func) (uint32_t, uint32_t)); 
    static inline uint32_t func_or (uint32_t a_, uint32_t b_);
    static inline uint32_t func_and (uint32_t a_, uint32_t b_);
    static inline uint32_t func_xor (uint32_t a_, uint32_t b_);
    static inline big_integer xor_bb(big_integer const & a, big_integer const & b); 
    static inline big_integer and_bb(big_integer const & a, big_integer const & b); 
    static inline big_integer or_bb(big_integer const & a, big_integer const & b); 
    
    static inline std::string to_string(big_integer s);
    static inline big_integer string_to_bi(std::string const & s);
    
    static inline big_integer div_bb(big_integer const & a, big_integer const & b, bool type = 0);
    static inline big_integer shr_bb(big_integer a, uint32_t b);
    static inline big_integer shl_bb(big_integer a, uint32_t b);
    
    static inline big_integer dec_postfix(big_integer & a);
    static inline big_integer & dec_prefix(big_integer & a);
    static inline big_integer inc_postfix(big_integer & a);
    static inline big_integer & inc_prefix(big_integer & a);

    friend const big_integer operator ~ (big_integer a);
    friend const big_integer operator - (big_integer a);
    friend const big_integer operator + (big_integer a);
    
    friend big_integer & operator -- (big_integer & a);
    friend big_integer operator -- (big_integer & a, int);
    friend big_integer & operator ++ (big_integer & a);
    friend big_integer operator ++ (big_integer & a, int);
    
    friend bool operator == (big_integer a, big_integer b);
    friend bool operator != (big_integer a, big_integer b);
    friend bool operator > (big_integer a, big_integer b);
    friend bool operator <= (big_integer a, big_integer b); 
    friend bool operator < (big_integer a, big_integer b);
    friend bool operator >= (big_integer a, big_integer b);
    
    friend big_integer operator + (big_integer a, big_integer b);
    friend big_integer operator - (big_integer a, big_integer b);
    friend big_integer operator * (big_integer a, big_integer b);
    friend big_integer operator / (big_integer a, big_integer b);
    friend big_integer operator % (big_integer a, big_integer b);
    friend big_integer operator ^ (big_integer a, big_integer b);
    friend big_integer operator & (big_integer a, big_integer b);
    friend big_integer operator | (big_integer a, big_integer b);
    friend big_integer operator >> (big_integer a, big_integer b);
    friend big_integer operator << (big_integer a, big_integer b);
    
    friend big_integer & operator += (big_integer & a, big_integer b);
    friend big_integer & operator -= (big_integer & a, big_integer b);
    friend big_integer & operator *= (big_integer & a, big_integer b);
    friend big_integer & operator /= (big_integer & a, big_integer b);
    friend big_integer & operator %= (big_integer & a, big_integer b);
    friend big_integer & operator ^= (big_integer & a, big_integer b);
    friend big_integer & operator &= (big_integer & a, big_integer b);
    friend big_integer & operator |= (big_integer & a, big_integer b);
    friend big_integer & operator >>= (big_integer & a, big_integer b);
    friend big_integer & operator <<= (big_integer & a, big_integer b);
};
*/
big_integer::big_integer (std::string s):
    size((s.size() + 8) / 9),
    signed_(0),
    data((s.size() + 8) / 9)
{
    big_integer::set_zero(*this);
    signed_ = (s[0] == '-');
    size_t i = (s[0] == '-') ? 1 : 0;
    for (; i != s.size(); ++i)
    {
        big_integer::mul_asign(*this, 10);
        //std::cout << *this << std::endl;
        big_integer::add_asign(*this, static_cast <uint32_t> (s[i] - '0'));
        //std::cout << *this << std::endl;
    }
}
big_integer & big_integer::operator = (big_integer const & a)
{
    if (a.data.data == data.data)
        return *this;
    if (size > 0)
        delete [] (data.data);
    data = big_obj(a.size);
    signed_ = a.signed_;
    for  (size_t i = 0; i != a.size; ++i)
        data.data[i] = a.data.data[i];
    return *this;
}
inline std::pair<bool, uint32_t> const big_integer::add(uint32_t const & a, uint32_t const & b, bool cf_)
{
    uint32_t pres = a + cf_;
    uint32_t res = pres + b;
    bool cf = (pres < a) || (res < pres) || (res < b);
    return std::make_pair(cf, res); 
}
inline std::pair<uint32_t, uint32_t> const big_integer::mul(uint32_t const & a, uint32_t const & b, uint32_t cf_, uint32_t cf__)   // a * b + cf_
{
    uint64_t res = ((uint64_t)a * (uint64_t)b) + (uint64_t)cf_ + (uint64_t)cf__;
    return std::make_pair((res >> 32), res & a11_);
}
inline bool big_integer::check_resize_bb(big_integer const & a, big_integer const & b)
{
    if (b.data.data[b.size - 1] < c11_ && b.data.data[b.size - 1] >= c01_)
        return 1;
    if (a.data.data[b.size - 1] < c11_ && a.data.data[b.size - 1] >= c01_)
        return 1;
    return 0;
}
inline big_integer const big_integer::add_bb (big_integer const & a, big_integer const & b, bool negate)
{
    bool need_resize = big_integer::check_resize_bb(a, b);
    big_integer res;
    size_t max_size = std::max(a.size, b.size) + need_resize;
    big_integer::set_size(res, max_size);
    bool cf = negate;
    uint32_t minus = (negate)? a11_ : a00_;
    size_t i;
    std::pair <bool, uint32_t> r;

    cf = negate;
    for (i = 0; i != std::min(a.size, b.size); ++i)
    {
        r = big_integer::add(a.data.data[i], b.data.data[i] ^ minus, cf);
        res.data.data[i] = r.second;
        cf = r.first;
    }
    uint32_t cb = minus;
    uint32_t ca = a00_;
    if (a.size > b.size)
    {
        for (; i != max_size - need_resize; ++i)
        {
            r = big_integer::add(a.data.data[i], cb, cf);
            res.data.data[i] = r.second;
            cf = r.first;
        }
    }
    else
    {
        for (; i != max_size - need_resize; ++i)
        {
            r = big_integer::add(b.data.data[i] ^ minus, ca, cf);
            res.data.data[i] = r.second;
            cf = r.first;
        }
    }
    if (need_resize)
    {
        r = big_integer::add(ca, cb, cf);
        res.data.data[max_size - 1] = r.second;
    }
    res.signed_ = a.signed_;
    return res;
}
inline void big_integer::set_zero(big_integer & a) //a BIG
{
    for (size_t i = 0; i != a.size; ++i)
        a.data.data[i] = 0;
}
inline void big_integer::add_mul(big_integer & a, big_integer const & b, uint32_t c, size_t pos) // a BIG shuld has correct size, b BIG  a += (b * c << pos);
{
    uint32_t cb = a00_;
    uint32_t cf = 0;
    std::pair <uint32_t, uint32_t> r;
    size_t i;
    for (i = 0; (i != b.size) && (i + pos != a.size); ++i)
    {
        r = big_integer::mul(c, b.data.data[i], cf, a.data.data[i + pos]);
        a.data.data[pos + i] = r.second;
        cf = r.first;
    }
    for (; (i + pos != a.size) && (cf); ++i)
    {
        r = big_integer::mul(c, cb, cf, a.data.data[i + pos]);
        a.data.data[pos + i] = r.second;
        cf = r.first;
    }
}
inline big_integer big_integer::mul_bb(big_integer const & a, big_integer const & b)
{
    big_integer res;
    size_t max_size = a.size + b.size;
    big_integer::set_size(res, max_size);
    big_integer::set_zero(res);
    size_t i;
    res.signed_ = a.signed_ ^ b.signed_;
    for (i = 0; i != b.size; ++i)
        big_integer::add_mul(res, a, b.data.data[i], i);
    return res;
}
inline void big_integer::mul_asign(big_integer & a, uint32_t const & b)  //a BIG with owerflow
{
    uint32_t cf = 0;
    for (size_t i = 0; i != a.size; ++i) 
    {
        std::pair <uint32_t, uint32_t> tmp = big_integer::mul(b, a.data.data[i], cf);
        cf = tmp.first;
        a.data.data[i] = tmp.second;
    }
}
inline void big_integer::add_asign(big_integer & a, uint32_t const & b)  //a BIG with owerflow
{
    std::pair <bool, uint32_t> tmp = big_integer::add(b, a.data.data[0], 0);
    bool cf = tmp.first;
    a.data.data[0] = tmp.second;
    for (size_t i = 1; i != a.size && cf; ++i) 
    {
        tmp = add(0, a.data.data[i], cf);
        cf = tmp.first;
        a.data.data[i] = tmp.second;
    }
}
inline uint32_t big_integer::div_asign(big_integer & a, uint32_t b)
{
    uint64_t divider = 0;
    for (size_t i = a.size - 1; i < a.size; --i)
    {
        divider = (divider << 32) + static_cast <uint64_t>(a.data.data[i]);
        a.data.data[i] = static_cast <uint32_t>(divider / static_cast <uint64_t>(b));
        divider %= b;
    }
    return static_cast <uint32_t> (divider);
}
bool big_integer::comp_fe(big_integer const & a, big_integer const & b) 
{
    size_t min_size = std::min(a.size, b.size);
    bool fa = 1;
    bool fb = 1;
    if (a.size > b.size)
        for (size_t i = a.size - 1; i >= min_size; --i)
            fa &= (a.data.data[i] == 0);
    else
        for (size_t i = b.size - 1; i >= min_size; --i)
            fb &= (b.data.data[i] == 0);
    for (size_t i = min_size - 1; i < min_size; --i)
        if (a.data.data[i] != b.data.data[i])
            return 0;
    return (fb && fa);
}   
bool big_integer::comp_fbe(big_integer const & a, big_integer const & b) 
{
    size_t min_size = std::min(a.size, b.size);
    bool fa = 0;
    bool fb = 1;
    if (a.size > b.size)
        for (size_t i = a.size - 1; i >= min_size; --i)
            fa |= (a.data.data[i] != 0);
    else
        for (size_t i = b.size - 1; i >= min_size; --i)
            fb &= (b.data.data[i] == 0);
    bool f = 1;
    for (size_t i = min_size - 1; i < min_size; --i)
    {
        if (a.data.data[i] > b.data.data[i])
            break;
        f &= (a.data.data[i] >= b.data.data[i]);
    }
    return (fb && (fa || f));
}
bool big_integer::comp_fb(big_integer const & a, big_integer const & b) 
{
    size_t min_size = std::min(a.size, b.size);
    bool fa = 0;
    bool fb = 1;
    if (a.size > b.size)
        for (size_t i = a.size - 1; i >= min_size; --i)
            fa |= (a.data.data[i] != 0);
    else
        for (size_t i = b.size - 1; i >= min_size; --i)
            fb &= (b.data.data[i] == 0);
    bool f;
    for (size_t i = min_size - 1; i < min_size; --i)
    {
        if (a.data.data[i] != b.data.data[i]) {
            f = a.data.data[i] > b.data.data[i];
            break;
        }
    }
    return (fb && (fa || f));
}
inline big_integer big_integer::bin_bb(big_integer const & a, big_integer const & b, uint32_t (*func) (uint32_t, uint32_t)) 
{
    big_integer res;
    size_t max_size = std::max(a.size, b.size);
    big_integer::set_size(res, max_size);
    uint32_t ca_ = a.signed_ != 0;
    uint32_t xa_ = (a.signed_ != 0)? big_integer::a11_ : big_integer::a00_;
    uint32_t cb_ = b.signed_ != 0;
    uint32_t xb_ = (b.signed_ != 0)? big_integer::a11_ : big_integer::a00_;
    for (size_t i = 0; i != std::min(a.size, b.size); ++i)
    {
        res.data.data[i] = func((a.data.data[i] ^ xa_) + ca_, (b.data.data[i] ^ xb_) + cb_);
        ca_ &= a.data.data[i] == 0;
        cb_ &= b.data.data[i] == 0;
    }
    if (c1_ & res.data.data[res.size - 1])
    {
        res.signed_ = 1;
        bool cr_ = 1;
        for (size_t i = 0; i != res.size; ++i)
        {
            res.data.data[i] = (res.data.data[i] ^ a11_) + cr_;
            cr_ &= res.data.data[i] == 0;
        }
    }
    return res;
}    
inline uint32_t big_integer::func_or (uint32_t a_, uint32_t b_) 
{
    return a_ | b_;
}
inline uint32_t big_integer::func_and (uint32_t a_, uint32_t b_) 
{
    return a_ & b_;
}
inline uint32_t big_integer::func_xor (uint32_t a_, uint32_t b_) 
{
    return a_ ^ b_;
}
inline big_integer big_integer::xor_bb(big_integer const & a, big_integer const & b) 
{
    return big_integer::bin_bb(a, b, &func_xor);
}
inline big_integer big_integer::and_bb(big_integer const & a, big_integer const & b) 
{
    return big_integer::bin_bb(a, b, &func_and);
}
inline big_integer big_integer::or_bb(big_integer const & a, big_integer const & b) 
{    
    return big_integer::bin_bb(a, b, (&func_or));
}
std::string to_string(big_integer const & ss)
{
    big_integer s(ss);
    if (big_integer::check_zero(s))
        return "0";
    std::string res = "";
    size_t max_size = s.size * 10;
    for (size_t i = 0; i != max_size; ++i)
    {
        uint32_t reminder = big_integer::div_asign(s, 10);
        res.push_back(static_cast <char> (reminder + '0'));
    }
    for (size_t i = max_size - 1; i < max_size && res[i] == '0'; --i)
    {
        res.pop_back();
    }
    if (res.size() == 0)
        res = "0";
    max_size = res.size();
    for (size_t i = 0; i != (max_size / 2); ++i)
    {
        std::swap(res[i], res[max_size - i - 1]);
    }
    if (s.signed_)
        res = "-" + res;
    return res;
}
/*
inline std::string big_integer::to_string(big_integer s)
{
    std::string res = "";
    size_t max_size = s.size * 10;
    for (size_t i = 0; i != max_size; ++i)
    {
        uint32_t reminder = big_integer::div_asign(s, 10);
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
*/
inline big_integer big_integer::string_to_bi(std::string const & s)
{
    big_integer res;
    size_t max_size = (s.size() + 8) / 9;
    big_integer::set_size(res, max_size);
    big_integer::set_zero(res);
    if (s.size() == 0)
        return res;
    res.signed_ = (s[0] == '-');
    size_t i = (s[0] == '-') ? 1 : 0;
    for (; i != s.size(); ++i)
    {
        big_integer::mul_asign(res, 10);
        big_integer::add_asign(res, static_cast <uint32_t> (s[i] - '0'));
    }
    return res;
}
void big_integer::sub_mul(big_integer & a, const big_integer & b, const uint32_t & c, const size_t & pos)
{
    uint64_t sub = 0;
    for (size_t i = 0; (i != b.size) && (i + pos != a.size); ++i)
    {
        sub += b.data.data[i] * c;
        a.data.data[i + pos] -= (sub & big_integer::a11_);
        sub = (sub >> 32) + ((a.data.data[i + pos] >= (sub & big_integer::a11_)) ? 0 : 1);
    }
}
inline big_integer big_integer::div_bb(big_integer & reminder, big_integer const & b, bool type)
{
    uint64_t digit = 0, d = 0; 
    size_t pos = 0;
    for (size_t i = 0; i != b.size; ++i)
        if (b.data.data[i] != 0)
        {
            pos = i;
            d = b.data.data[i];
        }
    big_integer res;
    big_integer muller;
    size_t max_size = reminder.size - pos;
    big_integer::set_size(res, max_size);
    big_integer::set_size(muller, max_size);
    res.signed_ = reminder.signed_ ^ b.signed_;

    for (size_t i = res.size - 1; i < res.size; --i)
    {
        digit |= reminder.data.data[i];
        uint32_t l = a00_;
        uint32_t r = a11_;
        //uint32_t m;
        /*
        while (r > l + 1)
        {
            big_integer::set_zero(muller);
            m = ((uint64_t)r + (uint64_t)l) >> 1;
            big_integer::add_mul(muller, b, m, i);
            if (big_integer::comp_fb(muller, reminder))
                r = m;
            else
                l = m;
        }
        */
        r = (digit / d);
        l = (r == 0) ? 0 : r - 1;
        big_integer::set_zero(muller);
        big_integer::add_mul(muller, b, r, i);
        if (big_integer::comp_fbe(reminder, muller))
            l = r;
        res.data.data[i] = l;
        //big_integer::set_zero(muller);
        //big_integer::add_mul(muller, b, l, i);
        //reminder = big_integer::add_bb(reminder, muller, 1);
        sub_mul(reminder, b, l, i);
        std::cout << reminder << std::endl;
        digit = (digit + reminder.data.data[i]) << 32;
    }
    if (!type)
        return res;
    else
        return reminder;
}
inline big_integer & big_integer::shr_bb(big_integer & a, uint32_t b) 
{
    size_t max_size = a.size - (b / 32) + 1;
    size_t tail = b % 32;
    size_t shift = b / 32;
    big_integer res;
    set_size(res, max_size);
    set_zero(res);
    res.signed_ = a.signed_;
    uint64_t tmp = 0;
    for (size_t i = a.size - 1; i != shift - 1; --i)
    {
        tmp |= a.data.data[i];
        res.data.data[i - shift] = (tmp >> tail) & big_integer::a11_;
        tmp = tmp << 32;
    }
    a = res;
    return a;
}
inline big_integer & big_integer::shl_bb(big_integer & a, uint32_t b) 
{
    size_t max_size = a.size + (b / 32) + 1;
    size_t tail = b % 32;
    size_t shift = b / 32;
    big_integer res;
    big_integer::set_size(res, max_size);
    big_integer::set_zero(res);
    res.signed_ = a.signed_;
    uint64_t tmp = 0;
    for (size_t i = a.size - 1; i < a.size; --i)
    {
        tmp |= a.data.data[i];
        res.data.data[i + shift] = (tmp << tail) & a11_;
        tmp = tmp << 32;
    }
    a = res;
    return a;
}
inline big_integer big_integer::dec_postfix(big_integer & a)
{
    size_t max_size = a.size;
    if (a.data.data[a.size - 1] & big_integer::c11_)
        ++max_size;
    big_integer res;
    res.signed_ = a.signed_;
    set_size(res, max_size);
    set_zero(res);
    bool cf = 1;
    for (size_t i = 0; i != res.size && cf; ++i)
    {
        res.data.data[i] = a.data.data[i] - 1;
        cf = res.data.data[i] == big_integer::a11_;
    }
    if (cf)
    {
        for (size_t i = 0; i != res.size && cf; ++i)
            res.data.data[i] = 0;
        res.data.data[0] = 1;
        res.signed_ = !res.signed_;
    }
    std::swap(a, res);
    return a;
}
inline big_integer & big_integer::dec_prefix(big_integer & a)
{
    if (a.data.data[a.size - 1] & big_integer::c11_)
    {
        big_integer res;
        res.signed_ = a.signed_;
        size_t max_size = a.size + 1;
        set_size(res, max_size);
        set_zero(res);
        bool cf = 1;
        for (size_t i = 0; i != res.size && cf; ++i)
        {
            res.data.data[i] = a.data.data[i] - 1;
            cf = res.data.data[i] == big_integer::a11_;
        }
        if (cf)
        {
            for (size_t i = 0; i != res.size && cf; ++i)
                res.data.data[i] = 0;
            res.data.data[0] = 1;
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
            --a.data.data[i];
            cf = a.data.data[i] == a11_;
        }
        if (cf)
        {
            for (size_t i = 0; i != a.size && cf; ++i)
                a.data.data[i] = 0;
            a.data.data[0] = 1;
            a.signed_ = !a.signed_;
        }
        return a;
    }
}
inline big_integer big_integer::inc_postfix(big_integer & a)
{
    size_t max_size = a.size;
    if (a.data.data[a.size - 1] & big_integer::c11_)
        ++max_size;
    big_integer res;
    res.signed_ = a.signed_;
    big_integer::set_size(res, max_size);
    big_integer::set_zero(res);
    bool cf = 1;
    for (size_t i = 0; i != res.size && cf; ++i)
    {
        res.data.data[i] = a.data.data[i] + 1;
        cf = res.data.data[i] == big_integer::a00_;
    }
    std::swap(a, res);
    return a;
}
inline big_integer & big_integer::inc_prefix(big_integer & a)
{
    if (a.data.data[a.size - 1] & big_integer::c11_)
    {
        big_integer res;
        res.signed_ = a.signed_;
        size_t max_size = a.size + 1;
        big_integer::set_size(res, max_size);
        big_integer::set_zero(res);
        bool cf = 1;
        for (size_t i = 0; i != res.size && cf; ++i)
        {
            res.data.data[i] = a.data.data[i] + 1;
            cf = res.data.data[i] == big_integer::a00_;
        }
        a = res;
        return a;
    }
    else
    {
        bool cf = 1;
        for (size_t i = 0; i != a.size && cf; ++i)
        {
            ++a.data.data[i];
            cf = a.data.data[i] == big_integer::a00_;
        }
        return a;
    }
}

void big_integer::set_size(big_integer & a, size_t siz)
{
    if (a.size > 0)
        delete [] a.data.data;
    a.size = siz;
    a.data = big_obj(siz);
}
    
void big_integer::swap (big_integer & a)
{
    std::swap(size, a.size);
    std::swap(signed_, a.signed_);
    std::swap(data, a.data);
}

const big_integer operator + (big_integer a)
{
    return a;
}
    
const big_integer operator - (big_integer a)
{
    a.signed_ ^= 1;
    return a;
}
    
big_integer & operator -- (big_integer & a)
{
    if (!a.signed_)
        return big_integer::dec_prefix(a);
    else
        return big_integer::inc_prefix(a);
}
    
big_integer operator -- (big_integer & a, int)
{
    if (!a.signed_)
        return big_integer::dec_postfix(a);
    else
        return big_integer::inc_postfix(a);
}
    
big_integer & operator ++ (big_integer & a)
{
    if (!a.signed_)
        return big_integer::inc_prefix(a);
    else
        return big_integer::dec_prefix(a);
}
    
big_integer operator ++ (big_integer & a, int)
{
    if (!a.signed_)
        return big_integer::inc_postfix(a);
    else
        return big_integer::dec_postfix(a);
}

bool big_integer::check_zero(big_integer const & a)
{
    for (size_t i = 0; i != a.size; ++i)
        if (a.data.data[i] != 0)
            return 0;
    return 1;
}

bool operator == (big_integer a, big_integer b)
{
    std::cout << a << " " << b << std::endl;
    return big_integer::comp_fe(a, b) && (a.signed_ == b.signed_ || big_integer::check_zero(a));
}

bool operator != (big_integer a, big_integer b)
{
    return !big_integer::comp_fe(a, b) || (a.signed_ != b.signed_ && !big_integer::check_zero(a));
}

bool operator > (big_integer a, big_integer b)
{
    return ((a.signed_== b.signed_ && (big_integer::comp_fb(a, b) ^ a.signed_)) || (!a.signed_ && b.signed_));
}

bool operator <= (big_integer a, big_integer b)
{
    return !((a.signed_== b.signed_ && (big_integer::comp_fb(a, b) ^ a.signed_)) || (!a.signed_ && b.signed_));
}

bool operator < (big_integer a, big_integer b)
{
    return !((a.signed_== b.signed_ && (big_integer::comp_fbe(a, b) ^ a.signed_)) || (!a.signed_ && b.signed_));
}

bool operator >= (big_integer a, big_integer b)
{
    return ((a.signed_== b.signed_ && (big_integer::comp_fbe(a, b) ^ a.signed_)) || (!a.signed_ && b.signed_));
}

big_integer operator - (big_integer a, big_integer b)
{
    if (a.signed_ != b.signed_)
    {
        return big_integer::add_bb(a, b, 0);
    }
    else
    {
        if (big_integer::comp_fb(a, b))
            return big_integer::add_bb(a, b, 1);
        else
        {
            b.signed_ ^= 1;
            return big_integer::add_bb(b, a, 1);
        }
    }
}
    
big_integer operator + (big_integer a, big_integer b)
{
    if (a.signed_ == b.signed_)
    {
        return big_integer::add_bb(a, b, 0);
    }
    else
    {
        if (big_integer::comp_fb(a, b))
            return big_integer::add_bb(a, b, 1);
        else
            return big_integer::add_bb(b, a, 1);
    }
}
    
big_integer operator * (big_integer a, big_integer b)
{
    return big_integer::mul_bb(a, b);
}
    
big_integer operator / (big_integer a, big_integer b)
{
    return big_integer::div_bb(a, b, 0);
}
    
big_integer operator % (big_integer a, big_integer b)
{
    return big_integer::div_bb(a, b, 1);
}
    
big_integer operator ^ (big_integer a, big_integer b)
{
    return big_integer::xor_bb(a, b);
}
    
big_integer operator | (big_integer a, big_integer b)
{
    return big_integer::or_bb(a, b);
}
    
big_integer operator & (big_integer a, big_integer b)
{
    return big_integer::and_bb(a, b);
}

const big_integer operator ~ (big_integer a)
{
    //std::cout << "a = " << a << std::endl;
    ++a;
    a.signed_ ^= 1;
    return a;
}

big_integer operator << (big_integer a, big_integer const & b)
{
    if (!b.signed_)
        return big_integer::shl_bb(a, b.data.data[0]);
    else
        return big_integer::shr_bb(a, b.data.data[0]);
}

big_integer operator >> (big_integer a, big_integer const & b)
{
    if (!b.signed_)
        return big_integer::shr_bb(a, b.data.data[0]);
    else
        return big_integer::shl_bb(a, b.data.data[0]);
}

std::ostream & operator << (std::ostream & s, big_integer const & a)
{
    s << "big_integer " << to_string(a);
    return s;
}
big_integer & operator += (big_integer & a, big_integer b)
{
    a = a + b;
    return a;
}
big_integer & operator -= (big_integer & a, big_integer b)
{
    a = a - b;
    return a;
}
big_integer & operator *= (big_integer & a, big_integer b)
{
    a = a * b;
    return a;
}
big_integer & operator /= (big_integer & a, big_integer b)
{
    a = a / b;
    return a;
}
big_integer & operator %= (big_integer & a, big_integer b)
{
    a= a % b;
    return a;
}
big_integer & operator ^= (big_integer & a, big_integer b)
{
    a = a ^ b;
    return a;
}
big_integer & operator &= (big_integer & a, big_integer b)
{
    a = a & b;
    return a;
}
big_integer & operator |= (big_integer & a, big_integer b)
{
    a = a | b;
    return a;
}
big_integer & operator >>= (big_integer & a, big_integer const & b)
{
    a = a >> b;
    return a;
}
big_integer & operator <<= (big_integer & a, big_integer const & b)
{
    a = a << b;
    return a;
}
/*
int main()
{
    big_integer a = big_integer::string_to_bi("-123456789");
    //std::cout << a.data.data[0] << std::endl;
    std::cout << big_integer::bi_to_string(a) << std::endl;
    big_integer b = big_integer::string_to_bi("100000000");
    std::cout << big_integer::bi_to_string(b) << std::endl;
    big_integer c;
    c = big_integer::add_bb(a, b, 1);
    std::cout << big_integer::bi_to_string(c) << std::endl;
    big_integer d;
    d = big_integer::mul_bb(a, b);
    std::string ss = big_integer::bi_to_string(d);
    std::cout << ss << std::endl;
    big_integer f;
    f = big_integer::div_bb(d, b);
    std::cout << big_integer::bi_to_string(f) << std::endl;
    
    
    big_integer a_ = big_integer::string_to_bi("400000000");
    big_integer b_ = big_integer::string_to_bi("400000000");
    big_integer d_ = big_integer::add_bb(a_, b_, 0);
    std::cout << big_integer::bi_to_string(d_) << std::endl;
    std::cout << big_integer::bi_to_string(big_integer::string_to_bi("10000000") + big_integer::string_to_bi("1000000")) << std::endl;
    big_integer a__ = big_integer::string_to_bi("10000000");
    big_integer b__ = big_integer::string_to_bi("1000000000");
    big_integer d__ = a__ + b__;
    std::cout << big_integer::bi_to_string(d__) << std::endl;*/
    /*
    big_integer a(std::make_pair(0xffffffff, 0xffffffff));
    big_integer b(std::make_pair(0xffffffff, 0xffffffff));
    std::cout << a << std::endl; 
    bool f = big_integer::check_resize_ss(a, b);
    a = big_integer::add_ss(a, b, 0);
    std::cout << a << std::endl;
    big_integer c = a;
    //std::cout << "kelelelleleeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeek" << std::endl;
    std::cout << a << std::endl;
    std::cout << c << std::endl;
    a = big_integer::mul_ss(a, c);
    //a = big_integer::mul_bb(a, c);
    //std::cout << a.data.small.left << "  " << a.data.small.rigth << "  ";
    std::cout << a << std::endl;
    std::cout << c << std::endl;
    std::cout << a.size << " " << f;
    */
//}















































