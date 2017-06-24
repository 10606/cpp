#pragma once
#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include "big_integer.h"
class big_integer;
class big_integer_no 
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
        big_obj():
            data(nullptr)
        {}
        big_obj(uint32_t * data_):
            data(data_)
        {}
        big_obj(size_t capacity_) 
        {
            if (capacity_ != 0)
            {
                data = new uint32_t [capacity_];
                memset(data, 0, capacity_ * 4);
            }
        }
    };
    
    size_t size;
    bool signed_;    
    big_obj data;
    ~big_integer_no();
    explicit big_integer_no (std::string val, uint32_t * dd);
    big_integer_no (uint32_t * const & data_, size_t const & size_, bool const & singed__);

private:
    static inline std::pair<bool, uint32_t> const add(uint32_t const & a, uint32_t const & b, bool cf_ = 0);
    static inline std::pair<uint32_t, uint32_t> const mul(uint32_t const & a, uint32_t const & b, uint32_t cf_ = 0, uint32_t cf__ = 0);

    static inline bool check_resize_bb(big_integer_no const & a, big_integer_no const & b);
    static inline void pre_add(big_integer_no const & a, big_integer_no & b, bool type_op, big_integer_no & res);
    static inline void add_bb (big_integer_no const & a, big_integer_no const & b, bool negate, big_integer_no & res);
    
    static inline bool check_zero(big_integer_no const & a);
    static inline void set_zero(big_integer_no & a);
    static inline void add_mul(big_integer_no & a, big_integer_no const & b, uint32_t c, size_t pos);
    
    static inline bool check_abecb(big_integer_no const & a, big_integer_no const & b, uint32_t const & c, size_t pos);
    static inline void sub_nr(big_integer_no & a, big_integer_no const & b, size_t const & pos);
    static inline void sub_mul(big_integer_no & a, big_integer_no const & b, uint32_t const & c, size_t const & pos);
    static inline void mul_asign(big_integer_no & a, uint32_t const & b);
    static inline void add_asign(big_integer_no & a, uint32_t const & b);
    static inline uint32_t div_asign(big_integer_no & a, uint32_t b);
    
    static bool comp_fe(big_integer_no const & a, big_integer_no const & b); 
    static bool comp_fbe(big_integer_no const & a, big_integer_no const & b); 
    static bool comp_fb(big_integer_no const & a, big_integer_no const & b);
    
    static inline void bin_bb(big_integer_no const & a, big_integer_no const & b, uint32_t (*func) (uint32_t const &, uint32_t const &), big_integer_no & res); 
    static inline uint32_t func_or (uint32_t const & a_, uint32_t const & b_);
    static inline uint32_t func_and (uint32_t const & a_, uint32_t const & b_);
    static inline uint32_t func_xor (uint32_t const & a_, uint32_t const & b_);
    
    static inline uint64_t pre_div_bb(big_integer_no & reminder, big_integer_no & b); //reminder and b cloned
    static inline void div_bb(big_integer_no & a, big_integer_no & b, big_integer_no & res, size_t pos, uint32_t d); //reminder and b cloned
    static inline void shr_bb(big_integer_no const & a, uint32_t const & b, big_integer_no & res);
    static inline void shl_bb(big_integer_no const & a, uint32_t const & b, big_integer_no & res);
    
    //a  cloned
    static inline void dec_postfix(big_integer_no & a, big_integer_no & res);
    static inline void dec_prefix(big_integer_no & a);
    static inline void inc_postfix(big_integer_no & a, big_integer_no & res);
    static inline void inc_prefix(big_integer_no & a);
    static inline void inc_prefix_ower(big_integer_no & a, big_integer_no & res);

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
    friend std::string to_string(big_integer_no const &);
    friend class big_integer;
    friend const big_integer_no operator ~ (big_integer_no a);
};
