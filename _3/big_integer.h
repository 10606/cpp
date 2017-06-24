#pragma once
#define cast_param(a) ((a.size > 4) ? a.data.big.get() : a.data.small), a.size, a.signed_
#define mkshared(a) sh_ptr (new uint32_t [a])
#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <memory>
#include "big_integer_no.h"
#include "sh_ptr.h"
class big_integer
{
public:
    static const uint32_t a00_ = 0;
    static const uint32_t a11_ = 0xffffffff;
    static const uint32_t c11_ = 0xc0000000;
    static const uint32_t c01_ = 0x40000000;
    static const uint32_t c1_ = 0x80000000;
    
    size_t size;
    bool signed_;    
    union big_small
    {
        sh_ptr big;
        uint32_t small[4];
        ~big_small()
        {}
        big_small():
            big()
        {}
        void assign (const big_small & a, bool t1, bool t2);//1 - big
    };
    big_small data;
    big_integer ();
    ~big_integer();
    big_integer (big_integer const & a);
    void swap (big_integer );
    big_integer & operator = (big_integer const & a);
    explicit big_integer (std::string val);
    big_integer (int32_t val);
private:
    uint32_t * clone()
    {
        if (size <= 4)
            return data.small;
        if (data.big.unique())
            return data.big.get();
        uint32_t * tmp = data.big.get();
        data.big.reset(new uint32_t [size]);
        for (size_t i = 0; i != size; ++i)
            data.big.get()[i] = tmp[i];
        return data.big.get();
    }
public:
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
    
    friend big_integer & operator -= (big_integer & a, big_integer b);
    friend big_integer & operator += (big_integer & a, big_integer b);
    friend big_integer & operator *= (big_integer & a, big_integer b);
    friend big_integer & operator /= (big_integer & a, big_integer b);
    friend big_integer & operator %= (big_integer & a, big_integer b);
    friend big_integer & operator ^= (big_integer & a, big_integer b);
    friend big_integer & operator |= (big_integer & a, big_integer b);
    friend big_integer & operator &= (big_integer & a, big_integer b);
    friend big_integer & operator <<= (big_integer & a, big_integer const & b);
    friend big_integer & operator >>= (big_integer & a, big_integer const & b);
    friend std::string to_string(big_integer);
    friend std::ostream & operator << (std::ostream & s, big_integer const & a);
};
