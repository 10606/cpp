#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>

#pragma once

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
            memset(data, 0, capacity_ * 4);
        }
        ~big_obj()
        {
            printf("%x\n", data);
            if (capacity != 0)
                delete [] (data);
        }
        big_obj & operator = (big_obj const & a)
        {
            if (capacity != 0)
                delete [] (data);
            data = a.data;
            capacity = a.capacity;
            return *this;
        }
    };
    
    size_t size;
    bool signed_;    
    big_obj data;
    big_integer ();
    ~big_integer();
    big_integer (big_integer const & a);
    void swap (big_integer &);
    static void set_size(big_integer &, size_t);
    big_integer & operator = (big_integer const & a);
    explicit big_integer (std::string val);
    big_integer (int32_t val);
    
    static inline std::pair<bool, uint32_t> const add(uint32_t const & a, uint32_t const & b, bool cf_ = 0);
    static inline std::pair<uint32_t, uint32_t> const mul(uint32_t const & a, uint32_t const & b, uint32_t cf_ = 0, uint32_t cf__ = 0);

    static inline bool check_resize_bb(big_integer const & a, big_integer const & b);
    static inline big_integer const add_bb (big_integer const & a, big_integer const & b, bool negate);
    
    static inline bool check_zero(big_integer const & a);
    static inline void set_zero(big_integer & a);
    static inline void add_mul(big_integer & a, big_integer const & b, uint32_t c, size_t pos);
    static inline big_integer mul_bb(big_integer const & a, big_integer const & b);
    
    static inline bool check_abecb(big_integer const & a, big_integer const & b, uint32_t const & c, size_t pos);
    static inline void sub_nr(big_integer & a, big_integer const & b, size_t const & pos);
    static inline void sub_mul(big_integer & a, big_integer const & b, uint32_t const & c, size_t const & pos);
    static inline void mul_asign(big_integer & a, uint32_t const & b);
    static inline void add_asign(big_integer & a, uint32_t const & b);
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
    
    static inline std::string bi_to_string(big_integer s);
    static inline big_integer string_to_bi(std::string const & s);
    
    static inline big_integer div_bb(big_integer & a, big_integer const & b, bool type);
    static inline big_integer & shr_bb(big_integer & a, uint32_t b);
    static inline big_integer & shl_bb(big_integer & a, uint32_t b);
    
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
    friend big_integer operator >> (big_integer a, big_integer const & b);
    friend big_integer operator << (big_integer a, big_integer const & b);
    
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
};
std::string to_string(big_integer const & ss);
