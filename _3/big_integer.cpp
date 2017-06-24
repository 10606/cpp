#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <memory>
#include "big_integer.h"
void big_integer::big_small::assign (const big_small & a, bool t1, bool t2)//1 - big
{
    if (t1)
    {
        if (t2)
            big.reset(a.big);
        else
        {
            big.~sh_ptr();
            for (size_t i = 0; i != 4; ++i)
                small[i] = a.small[i];
        }
    }
    else
    {
        if (t2)
            big.set(a.big);
        else
        {
            for (size_t i = 0; i != 4; ++i)
                small[i] = a.small[i];
        }
    }
}
std::ostream & operator << (std::ostream & s, big_integer const & a)
{
    s << to_string(a);
    return s;
}
big_integer::big_integer ():
    size(1),
    signed_(0)
{
    memset(data.small, 0, 16);
}
big_integer::~big_integer()
{
    if (size > 4)
        data.big.~sh_ptr();
}
big_integer::big_integer (big_integer const & a):
    size(a.size),
    signed_(a.signed_)
{
    if (a.size <= 4)
    {
        for (size_t i = 0; i != 4; ++i)
            data.small[i] = a.data.small[i];
    }
    else
        data.big.set(a.data.big);
}
void big_integer::swap (big_integer a)
{   
    data.assign(a.data, size > 4, a.size > 4);
    size = a.size;
    std::swap(signed_, a.signed_);
}
big_integer & big_integer::operator = (big_integer const & a)
{
    swap(a);
    return *this;
}
big_integer::big_integer (int32_t val):
    size(1),
    signed_(val < 0)
{
    data.small[0] = (val < 0) ? (-static_cast <uint32_t> (val)) : (static_cast <uint32_t> (val));
}
big_integer_no::big_integer_no (std::string s, uint32_t * dd):
    size((s.size() + 8) / 9),
    signed_(0),
    data(dd)
{
    big_integer_no::set_zero(*this);
    signed_ = (s[0] == '-');
    size_t i = (s[0] == '-') ? 1 : 0;
    for (; i != s.size(); ++i)
    {
        big_integer_no::mul_asign(*this, 10);
        big_integer_no::add_asign(*this, static_cast <uint32_t> (s[i] - '0'));
    }
}
big_integer::big_integer (std::string val):
    size((val.size() + 8) / 9),
    signed_(0)
{
    signed_ = (val[0] == '-');
    if (size > 4)
    {
        data.big.set(mkshared(size));
        big_integer_no tmp(val, data.big.get());
    }
    else
    {
        memset(data.small, 0, 16);
        big_integer_no tmp(val, data.small);
    }
}
std::string to_string(big_integer ss)
{
    ss.clone();
    big_integer_no s(cast_param(ss));
    if (big_integer_no::check_zero(s))
        return "0";
    std::string res = "";
    size_t max_size = s.size * 10;
    for (size_t i = 0; i != max_size; ++i)
    {
        uint32_t reminder = big_integer_no::div_asign(s, 10);
        res.push_back(static_cast <char> (reminder + '0'));
    }
    for (size_t i = max_size - 1; i < max_size && res[i] == '0'; --i)
        res.pop_back();
    if (res.size() == 0)
        res = "0";
    max_size = res.size();
    for (size_t i = 0; i != (max_size / 2); ++i)
        std::swap(res[i], res[max_size - i - 1]);
    if (s.signed_)
        res = "-" + res;
    return res;
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
big_integer & operator -- (big_integer & aa)
{
    aa.clone();
    big_integer_no a(cast_param(aa));
    if (!a.signed_)
        big_integer_no::dec_prefix(a);
    else
    {
        if (a.data.data[a.size - 1] & big_integer_no::c11_)
            big_integer_no::inc_prefix(a);
        else
        {
            size_t max_size = a.size + 1;
            big_integer tmp;
            tmp.size = max_size;
            tmp.signed_ = a.signed_;
            if (max_size > 4)
                tmp.data.big.set(mkshared(max_size));
            big_integer_no res(cast_param(tmp));
            big_integer_no::set_zero(res);
            big_integer_no::inc_prefix_ower(a, res);
            std::swap(aa, tmp);
        }
    }
    aa.signed_ = a.signed_;
    return aa;
}
big_integer operator -- (big_integer & aa, int)
{
    aa.clone();
    big_integer_no a(cast_param(aa));
    size_t max_size = a.size;
    if (a.data.data[a.size - 1] & big_integer_no::c11_)
        ++max_size;
    big_integer tmp;
    tmp.size = max_size;
    tmp.signed_ = a.signed_;
    if (max_size > 4)
        tmp.data.big.set(mkshared(max_size));
    big_integer_no res(cast_param(tmp));
    big_integer_no::set_zero(res);
    if (!a.signed_)
        big_integer_no::dec_postfix(a, res);
    else
        big_integer_no::inc_postfix(a, res);
    std::swap(aa, tmp);
    aa.signed_ = a.signed_;
    return aa;
}
big_integer & operator ++ (big_integer & aa)
{
    aa.clone();
    big_integer_no a(cast_param(aa));
    if (!a.signed_)
    {
        if (a.data.data[a.size - 1] & big_integer_no::c11_)
            big_integer_no::inc_prefix(a);
        else
        {
            size_t max_size = a.size + 1;
            big_integer tmp;
            tmp.size = max_size;
            tmp.signed_ = a.signed_;
            if (max_size > 4)
                tmp.data.big.set(mkshared(max_size));
            big_integer_no res(cast_param(tmp));
            big_integer_no::set_zero(res);
            big_integer_no::inc_prefix_ower(a, res);
            std::swap(aa, tmp);
        }
    }
    else
        big_integer_no::dec_prefix(a);
    aa.signed_ = a.signed_;
    return aa;
}
big_integer operator ++ (big_integer & aa, int)
{
    aa.clone();
    big_integer_no a(cast_param(aa));
    size_t max_size = a.size;
    if (a.data.data[a.size - 1] & big_integer_no::c11_)
        ++max_size;
    big_integer tmp;
    tmp.size = max_size;
    tmp.signed_ = a.signed_;
    if (max_size > 4)
        tmp.data.big.set(mkshared(max_size));
    big_integer_no res(cast_param(tmp));
    big_integer_no::set_zero(res);
    if (!a.signed_)
        big_integer_no::inc_postfix(a, res);
    else
        big_integer_no::dec_postfix(a, res);
    std::swap(aa, tmp);
    aa.signed_ = a.signed_;
    return aa;
}
bool operator == (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    return big_integer_no::comp_fe(a, b) && (a.signed_ == b.signed_ || big_integer_no::check_zero(a));
}
bool operator != (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    return !big_integer_no::comp_fe(a, b) || (a.signed_ != b.signed_ && !big_integer_no::check_zero(a));
}
bool operator > (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    return ((a.signed_== b.signed_ && (big_integer_no::comp_fb(a, b) ^ a.signed_)) || (!a.signed_ && b.signed_));
}
bool operator <= (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    return !((a.signed_== b.signed_ && (big_integer_no::comp_fb(a, b) ^ a.signed_)) || (!a.signed_ && b.signed_));
}
bool operator < (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    return !((a.signed_== b.signed_ && (big_integer_no::comp_fbe(a, b) ^ a.signed_)) || (!a.signed_ && b.signed_));
}
bool operator >= (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    return ((a.signed_== b.signed_ && (big_integer_no::comp_fbe(a, b) ^ a.signed_)) || (!a.signed_ && b.signed_));
}
big_integer operator - (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    bool need_resize = big_integer_no::check_resize_bb(a, b);
    size_t max_size = std::max(a.size, b.size) + need_resize;
    big_integer tmp;
    tmp.size = max_size;
    tmp.signed_ = a.signed_;
    if (max_size > 4)
        tmp.data.big.set(mkshared(max_size));
    big_integer_no res(cast_param(tmp));
    big_integer_no::pre_add(a, b, 1, res);
    tmp.signed_ = res.signed_;
    return tmp;
}
big_integer operator + (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    bool need_resize = big_integer_no::check_resize_bb(a, b);
    size_t max_size = std::max(a.size, b.size) + need_resize;
    big_integer tmp;
    tmp.size = max_size;
    tmp.signed_ = a.signed_;
    if (max_size > 4)
        tmp.data.big.set(mkshared(max_size));
    big_integer_no res(cast_param(tmp));
    big_integer_no::pre_add(a, b, 0, res);
    tmp.signed_ = res.signed_;
    return tmp;
}
big_integer operator * (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    size_t max_size = a.size + b.size;
    big_integer tmp;
    tmp.size = max_size;
    tmp.signed_ = a.signed_;
    if (max_size > 4)
        tmp.data.big.set(mkshared(max_size));
    big_integer_no res(cast_param(tmp));
    big_integer_no::set_zero(res);
    size_t i;
    tmp.signed_ = a.signed_ ^ b.signed_;
    for (i = 0; i != b.size; ++i)
        big_integer_no::add_mul(res, a, b.data.data[i], i);
    return tmp;
}
big_integer operator / (big_integer aa, big_integer bb)
{
    aa.clone();
    bb.clone();
    big_integer_no a_(cast_param(aa));
    big_integer_no b_(cast_param(bb));
    uint64_t norm;
    norm = big_integer_no::pre_div_bb(a_, b_);
    aa *= static_cast <uint32_t> (norm);
    bb *= static_cast <uint32_t> (norm);
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    uint32_t d;
    size_t pos = 0;
    for (size_t i = 0; i != b.size; ++i)
        if (b.data.data[i] != 0)
        {
            pos = i;
            d = b.data.data[i];
        }
    size_t max_size = a.size - pos;
    big_integer tmp;
    tmp.size = max_size;
    tmp.signed_ = a.signed_;
    if (max_size > 4)
        tmp.data.big.set(mkshared(max_size));
    big_integer_no res(cast_param(tmp));
    big_integer_no::div_bb(a, b, res, d, pos);
    tmp.signed_ = res.signed_;
    return tmp;
}
big_integer operator % (big_integer a, big_integer b)
{
    return (a - a / b * b);
}
big_integer operator ^ (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    size_t max_size = std::max(a.size, b.size);
    big_integer tmp;
    tmp.size = max_size;
    tmp.signed_ = a.signed_;
    if (max_size > 4)
        tmp.data.big.set(mkshared(max_size));
    big_integer_no res(cast_param(tmp));
    big_integer_no::bin_bb(a, b, &big_integer_no::func_xor, res);
    tmp.signed_ = res.signed_;
    return tmp;
}
big_integer operator | (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    size_t max_size = std::max(a.size, b.size);
    big_integer tmp;
    tmp.size = max_size;
    tmp.signed_ = a.signed_;
    if (max_size > 4)
        tmp.data.big.set(mkshared(max_size));
    big_integer_no res(cast_param(tmp));
    big_integer_no::bin_bb(a, b, &big_integer_no::func_or, res);
    tmp.signed_ = res.signed_;
    return tmp;
}
big_integer operator & (big_integer aa, big_integer bb)
{
    big_integer_no a(cast_param(aa));
    big_integer_no b(cast_param(bb));
    size_t max_size = std::max(a.size, b.size);
    big_integer tmp;
    tmp.size = max_size;
    tmp.signed_ = a.signed_;
    if (max_size > 4)
        tmp.data.big.set(mkshared(max_size));
    big_integer_no res(cast_param(tmp));
    big_integer_no::bin_bb(a, b, &big_integer_no::func_and, res);
    tmp.signed_ = res.signed_;
    return tmp;
}
const big_integer operator ~ (big_integer a)
{
    a++;
    a.signed_ ^= 1;
    return a;
}
big_integer operator << (big_integer aa, big_integer bb)
{
    if (!bb.signed_)
    {
        big_integer_no a(cast_param(aa));
        big_integer_no b(cast_param(bb));
        size_t max_size = a.size + (b.data.data[0] / 32) + 1;
        big_integer tmp;
        tmp.size = max_size;
        tmp.signed_ = a.signed_;
        if (max_size > 4)
            tmp.data.big.set(mkshared(max_size));
        big_integer_no res(cast_param(tmp));
        big_integer_no::set_zero(res);
        big_integer_no::shl_bb(a, b.data.data[0], res);
        tmp.signed_ = res.signed_;
        return tmp;
    }
    else
    {
        size_t max_size = aa.size - (((bb.size > 4) ? bb.data.big.get()[0] : bb.data.small[0]) / 32) + 1;
        big_integer tmp;
        tmp.size = max_size;
        tmp.signed_ = aa.signed_;
        if (max_size > 4)
            tmp.data.big.set(mkshared(max_size));
        big_integer_no res(cast_param(tmp));
        big_integer_no::set_zero(res);
        if (!aa.signed_)
        {
            big_integer_no a(cast_param(aa));
            big_integer_no b(cast_param(bb));
            big_integer_no::shr_bb(a, b.data.data[0], res);
            tmp.signed_ = res.signed_;
        }
        else
        {
            aa = ~aa;
            tmp.signed_ ^= 1;
            big_integer_no a(cast_param(aa));
            big_integer_no b(cast_param(bb));
            big_integer_no::shr_bb(a, b.data.data[0], res);
            tmp = ~tmp;
        }
        return tmp;
    }
}
big_integer operator >> (big_integer aa, big_integer bb)
{
    if (!bb.signed_)
    {
        size_t max_size = aa.size - (((bb.size > 4) ? bb.data.big.get()[0] : bb.data.small[0]) / 32) + 1;
        big_integer tmp;
        tmp.size = max_size;
        tmp.signed_ = aa.signed_;
        if (max_size > 4)
            tmp.data.big.set(mkshared(max_size));
        big_integer_no res(cast_param(tmp));
        big_integer_no::set_zero(res);
        if (!aa.signed_)
        {
            big_integer_no a(cast_param(aa));
            big_integer_no b(cast_param(bb));
            big_integer_no::shr_bb(a, b.data.data[0], res);
            tmp.signed_ = res.signed_;
        }
        else
        {
            aa = ~aa;
            big_integer_no a(cast_param(aa));
            big_integer_no b(cast_param(bb));
            tmp.signed_ ^= 1;
            big_integer_no::shr_bb(a, b.data.data[0], res);
            tmp = ~tmp;
        }
        return tmp;
    }
    else
    {
        big_integer_no a(cast_param(aa));
        big_integer_no b(cast_param(bb));
        size_t max_size = a.size + (b.data.data[0] / 32) + 1;
        big_integer tmp;
        tmp.size = max_size;
        tmp.signed_ = a.signed_;
        if (max_size > 4)
            tmp.data.big.set(mkshared(max_size));
        big_integer_no res(cast_param(tmp));
        big_integer_no::set_zero(res);
        big_integer_no::shl_bb(a, b.data.data[0], res);
        tmp.signed_ = res.signed_;
        return tmp;
    }
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
big_integer_no::~big_integer_no()
{}
big_integer_no::big_integer_no (uint32_t * const & data_, size_t const & size_, bool const & signed__):
    size(size_),
    signed_(signed__),
    data(data_)
{}
inline std::pair<bool, uint32_t> const big_integer_no::add(uint32_t const & a, uint32_t const & b, bool cf_)
{
    uint32_t pres = a + cf_;
    uint32_t res = pres + b;
    bool cf = (pres < a) || (res < pres) || (res < b);
    return std::make_pair(cf, res); 
}
inline std::pair<uint32_t, uint32_t> const big_integer_no::mul(uint32_t const & a, uint32_t const & b, uint32_t cf_, uint32_t cf__)   // a * b + cf_
{
    uint64_t res = ((uint64_t)a * (uint64_t)b) + (uint64_t)cf_ + (uint64_t)cf__;
    return std::make_pair((res >> 32), res & a11_);
}
inline bool big_integer_no::check_resize_bb(big_integer_no const & a, big_integer_no const & b)
{
    if (b.data.data[b.size - 1] >= c01_)
        return 1;
    if (a.data.data[a.size - 1] >= c01_)
        return 1;
    return 0;
}
void big_integer_no::pre_add(big_integer_no const & a, big_integer_no & b, bool type_op, big_integer_no & res)
{
    if ((a.signed_ == b.signed_) ^ type_op)
        big_integer_no::add_bb(a, b, 0, res);
    else
    {
        if (big_integer_no::comp_fb(a, b))
            big_integer_no::add_bb(a, b, 1, res);
        else
        {
            if (type_op)
                b.signed_ ^= 1;
            big_integer_no::add_bb(b, a, 1, res);
        }
    }
}
inline void big_integer_no::add_bb (big_integer_no const & a, big_integer_no const & b, bool negate, big_integer_no & res)
{
    bool cf = negate;
    uint32_t minus = (negate)? a11_ : a00_;
    size_t i;
    std::pair <bool, uint32_t> r;

    cf = negate;
    for (i = 0; i != std::min(a.size, b.size); ++i)
    {
        r = big_integer_no::add(a.data.data[i], b.data.data[i] ^ minus, cf);
        res.data.data[i] = r.second;
        cf = r.first;
    }
    uint32_t cb = minus;
    uint32_t ca = a00_;
    if (a.size > b.size)
    {
        for (; i != a.size; ++i)
        {
            r = big_integer_no::add(a.data.data[i], cb, cf);
            res.data.data[i] = r.second;
            cf = r.first;
        }
    }
    else
    {
        for (; i != b.size; ++i)
        {
            r = big_integer_no::add(b.data.data[i] ^ minus, ca, cf);
            res.data.data[i] = r.second;
            cf = r.first;
        }
    }
    for (; i != res.size; ++i)
    {
        r = big_integer_no::add(ca, cb, cf);
        res.data.data[i] = r.second;
        cf = r.first;
    }
    res.signed_ = a.signed_;
}
inline void big_integer_no::set_zero(big_integer_no & a)
{
    memset(a.data.data, 0, a.size * 4);
}
inline void big_integer_no::add_mul(big_integer_no & a, big_integer_no const & b, uint32_t c, size_t pos)
{
    uint32_t cb = a00_;
    uint32_t cf = 0;
    std::pair <uint32_t, uint32_t> r;
    size_t i;
    for (i = 0; (i != b.size) && (i + pos != a.size); ++i)
    {
        r = big_integer_no::mul(c, b.data.data[i], cf, a.data.data[i + pos]);
        a.data.data[pos + i] = r.second;
        cf = r.first;
    }
    for (; (i + pos != a.size) && (cf); ++i)
    {
        r = big_integer_no::mul(c, cb, cf, a.data.data[i + pos]);
        a.data.data[pos + i] = r.second;
        cf = r.first;
    }
}
inline void big_integer_no::mul_asign(big_integer_no & a, uint32_t const & b)
{
    uint32_t cf = 0;
    for (size_t i = 0; i != a.size; ++i) 
    {
        std::pair <uint32_t, uint32_t> tmp = big_integer_no::mul(b, a.data.data[i], cf);
        cf = tmp.first;
        a.data.data[i] = tmp.second;
    }
}
inline void big_integer_no::add_asign(big_integer_no & a, uint32_t const & b)
{
    std::pair <bool, uint32_t> tmp = big_integer_no::add(b, a.data.data[0], 0);
    bool cf = tmp.first;
    a.data.data[0] = tmp.second;
    for (size_t i = 1; i != a.size && cf; ++i) 
    {
        tmp = add(0, a.data.data[i], cf);
        cf = tmp.first;
        a.data.data[i] = tmp.second;
    }
}
inline uint32_t big_integer_no::div_asign(big_integer_no & a, uint32_t b)
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
bool big_integer_no::check_zero(big_integer_no const & a)
{
    for (size_t i = 0; i != a.size; ++i)
        if (a.data.data[i] != 0)
            return 0;
    return 1;
}
bool big_integer_no::comp_fe(big_integer_no const & a, big_integer_no const & b) 
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
bool big_integer_no::comp_fbe(big_integer_no const & a, big_integer_no const & b) 
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
bool big_integer_no::comp_fb(big_integer_no const & a, big_integer_no const & b) 
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
    bool f = 0;
    for (size_t i = min_size - 1; i < min_size; --i)
    {
        if (a.data.data[i] != b.data.data[i]) 
        {
            f = a.data.data[i] > b.data.data[i];
            break;
        }
    }
    return (fb && (fa || f));
}
void big_integer_no::sub_mul(big_integer_no & a, const big_integer_no & b, const uint32_t & c, const size_t & pos)
{
    uint64_t sub = 0;
    size_t i = 0;
    for (; (i != b.size) && (i + pos != a.size); ++i)
    {
        sub += static_cast <uint64_t> (b.data.data[i]) * static_cast <uint64_t> (c);
        uint64_t f = ((a.data.data[i + pos] >= (sub & big_integer_no::a11_)) ? 0 : 1) + ((sub) >> 32);
        a.data.data[i + pos] -= (sub & big_integer_no::a11_);
        sub = f;
    }
    for (; (i + pos != a.size) && (sub != 0); ++i)
    {
        uint64_t f = ((a.data.data[i + pos] >= (sub & big_integer_no::a11_)) ? 0 : 1) + ((sub) >> 32);
        a.data.data[i + pos] -= sub;
        sub = f;
    }
}
bool big_integer_no::check_abecb(const big_integer_no & a, const big_integer_no & b, const uint32_t & c, size_t pos)
{
    uint64_t sub = 0;
    size_t i = 0;
    bool flag = 1;
    for (; (i != b.size) && (i + pos != a.size); ++i)
    {
        sub += static_cast <uint64_t> (b.data.data[i]) * static_cast <uint64_t> (c);
        if (a.data.data[i + pos] < (sub & big_integer_no::a11_))
            flag = 0;
        if (a.data.data[i + pos] > (sub & big_integer_no::a11_))
            flag = 1;
        sub = sub >> 32;
    }
    if ((i + pos != a.size) && (a.data.data[i + pos] < sub))
        flag = 0;
    for (; (i + pos != a.size) && (!flag); ++i)
    {
        if (a.data.data[i + pos] > sub)
            return 1;
        sub = sub >> 32;
    }
    if (a.size != i + pos)
        sub = sub >> 32;
    flag &= (sub == 0);
    return flag;
}
inline uint64_t big_integer_no::pre_div_bb(big_integer_no & reminder, big_integer_no & b) //reminder and b cloned
{
    uint64_t d = 0; 
    for (size_t i = 0; i != b.size; ++i)
        if (b.data.data[i] != 0)
            d = b.data.data[i];
    uint64_t norm = std::max((static_cast <uint64_t> (big_integer_no::a11_) / (d + 1)), static_cast <uint64_t> (1));
    return norm;
}
inline void big_integer_no::div_bb(big_integer_no & reminder, big_integer_no & b, big_integer_no & res, size_t d, uint32_t pos) //reminder and b cloned
{
    uint64_t digit = 0; 
    res.signed_ = reminder.signed_ ^ b.signed_;

    for (size_t i = res.size - 1; i < res.size; --i)
    {
        digit |= reminder.data.data[i + pos];
        uint64_t r = std::min(digit / d, static_cast <uint64_t> (big_integer_no::a11_));
        while (!(big_integer_no::check_abecb(reminder, b, r, i)))
            r--;
        res.data.data[i] = r;
        sub_mul(reminder, b, r, i);
        digit = static_cast <uint64_t> (reminder.data.data[i + pos]) << 32;
    }

}
inline void big_integer_no::shr_bb(big_integer_no const & a, uint32_t const & b, big_integer_no & res) 
{
    size_t tail = b % 32;
    size_t shift = b / 32;
    res.signed_ = a.signed_;
    uint64_t tmp = 0;
    for (size_t i = a.size - 1; i != shift - 1; --i)
    {
        tmp |= a.data.data[i];
        res.data.data[i - shift] = (tmp >> tail) & big_integer_no::a11_;
        tmp = tmp << 32;
    }
}
inline void big_integer_no::shl_bb(big_integer_no const & a, uint32_t const & b, big_integer_no & res) 
{
    size_t tail = b % 32;
    size_t shift = b / 32;
    res.signed_ = a.signed_;
    uint64_t tmp = 0;
    for (size_t i = a.size - 1; i < a.size; --i)
    {
        tmp |= a.data.data[i];
        res.data.data[i + shift] = (tmp << tail) & a11_;
        tmp = tmp << 32;
    }
}
inline void big_integer_no::dec_postfix(big_integer_no & a, big_integer_no & res) //a cloned
{
    bool cf = 1;
    size_t i = 0;
    for (; i != res.size && cf; ++i)
    {
        res.data.data[i] = a.data.data[i] - 1;
        cf = (res.data.data[i] == big_integer_no::a11_);
    }
    for (; i != a.size; ++i)
        res.data.data[i] = a.data.data[i];
    if (cf)
    {
        for (size_t i = 0; i != res.size && cf; ++i)
            res.data.data[i] = 0;
        res.data.data[0] = 1;
        res.signed_ = !res.signed_;
    }
    std::swap(a.size, res.size);
    std::swap(a.signed_, res.signed_);
    std::swap(a.data.data, res.data.data);
}
inline void big_integer_no::dec_prefix(big_integer_no & a) //a cloned
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
}
inline void big_integer_no::inc_postfix(big_integer_no & a, big_integer_no & res) //a cloned
{
    bool cf = 1;
    size_t i = 0;
    for (; i != res.size && cf; ++i)
    {
        res.data.data[i] = a.data.data[i] + 1;
        cf = res.data.data[i] == big_integer_no::a00_;
    }
    for (; i != a.size; ++i)
        res.data.data[i] = a.data.data[i];
    std::swap(a.size, res.size);
    std::swap(a.signed_, res.signed_);
    std::swap(a.data.data, res.data.data);
}
inline void big_integer_no::inc_prefix_ower(big_integer_no & a, big_integer_no & res) //a cloned
{
    bool cf = 1;
    size_t i = 0;
    for (; i != res.size && cf; ++i)
    {
        res.data.data[i] = a.data.data[i] + 1;
        cf = res.data.data[i] == big_integer_no::a00_;
    }
    for (; i != a.size; ++i)
        res.data.data[i] = a.data.data[i];
    std::swap(a.size, res.size);
    std::swap(a.signed_, res.signed_);
    std::swap(a.data.data, res.data.data);
}
inline void big_integer_no::inc_prefix(big_integer_no & a) //a cloned
{
    bool cf = 1;
    for (size_t i = 0; i != a.size && cf; ++i)
    {
        ++a.data.data[i];
        cf = a.data.data[i] == big_integer_no::a00_;
    }
}
inline void big_integer_no::bin_bb(big_integer_no const & a, big_integer_no const & b, uint32_t (*func) (uint32_t const &, uint32_t const &), big_integer_no & res) 
{
    uint32_t ca_ = a.signed_ != 0;
    uint32_t xa_ = (a.signed_ != 0)? big_integer_no::a11_ : big_integer_no::a00_;
    uint32_t cb_ = b.signed_ != 0;
    uint32_t xb_ = (b.signed_ != 0)? big_integer_no::a11_ : big_integer_no::a00_;
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
}    
std::string to_string(big_integer_no const & ss)
{
    big_integer_no s(ss);
    if (big_integer_no::check_zero(s))
        return "0";
    std::string res = "";
    size_t max_size = s.size * 10;
    for (size_t i = 0; i != max_size; ++i)
    {
        uint32_t reminder = big_integer_no::div_asign(s, 10);
        res.push_back(static_cast <char> (reminder + '0'));
    }
    for (size_t i = max_size - 1; i < max_size && res[i] == '0'; --i)
        res.pop_back();
    if (res.size() == 0)
        res = "0";
    max_size = res.size();
    for (size_t i = 0; i != (max_size / 2); ++i)
        std::swap(res[i], res[max_size - i - 1]);
    if (s.signed_)
        res = "-" + res;
    return res;
}
inline uint32_t big_integer_no::func_or (uint32_t const & a_, uint32_t const & b_) 
{
    return a_ | b_;
}
inline uint32_t big_integer_no::func_and (uint32_t const & a_, uint32_t const & b_) 
{
    return a_ & b_;
}
inline uint32_t big_integer_no::func_xor (uint32_t const & a_, uint32_t const & b_) 
{
    return a_ ^ b_;
}
