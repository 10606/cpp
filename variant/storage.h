#ifndef STORAGE_H
#define STORAGE_H

#include <utility>
#include <algorithm>
#include <cstdio>
#include <memory>
#include <exception>
#include <type_traits>
#include <limits>
#include <algorithm>
#include <assert.h>

#include "somethig.h"

template <bool f, typename T0, typename ... Ts>
struct storage
{
public:
    union 
    {
        T0 value;
        storage <f, Ts ...> tail;
    };
    constexpr storage ()
    {
        new (&value) T0();
    }
    template <typename  T, size_t I>
    constexpr T & set (std::integral_constant <size_t, I>, T && t)
    {
        return tail.template set (std::integral_constant <size_t, I - 1>(), std::forward <T> (t));
    }
    template <typename  T>
    constexpr T & set (std::integral_constant <size_t, 0>, T && t)
    {
        assert ((is_same <T, T0> :: value));
        new (&value) T0(std::forward <T> (t));
        return value;
    }
    template <size_t I, typename ... Args>
    constexpr typename get_type <I, T0, Ts ...> :: type & set (std::integral_constant <size_t, I>, Args && ... args)
    {
        return tail.template set (std::integral_constant <size_t, I - 1>(), std::forward <Args> (args) ...);
    }
    template <typename ... Args>
    constexpr typename get_type <0, T0> :: type & set (std::integral_constant <size_t, 0>, Args && ... args)
    {
        new (&value) T0(std::forward <Args> (args) ...);
        return value;
    }
    //constructor
    template <size_t I, typename  T>
    constexpr storage (std::integral_constant <size_t, I>, T && t) :
        tail(std::integral_constant <size_t, I - 1>(),  std::forward <T> (t))
    {}
    template <typename  T>
    constexpr storage (std::integral_constant <size_t, 0>, T && t) :
        value(std::forward <T> (t))
    {}
    
    template <size_t I, typename  T, typename ... Args>
    constexpr storage (std::integral_constant <size_t, I>, std::in_place_type_t <T>, Args && ... args) :
        tail(std::integral_constant <size_t, I - 1>(), std::in_place_type_t <T> (), std::forward <Args> (args)...)
    {}
    template <typename  T, typename ... Args>
    constexpr storage (std::integral_constant <size_t, 0>, std::in_place_type_t <T>, Args && ... args) :
        value(std::forward <Args> (args) ...)
    {}

    template <size_t I, typename ... Args>
    constexpr storage (std::in_place_index_t <I>, Args && ... args) :
        tail(std::in_place_index_t <I - 1> (), std::forward <Args> (args)...)
    {}
    template <typename ... Args>
    constexpr storage (std::in_place_index_t <0>, Args && ... args) :
        value(std::forward <Args> (args) ...)
    {}
    //destroy
    constexpr void destroy (size_t I)
    {
        if (I == 0)
            value.~T0();
        else
            tail.destroy(I - 1);
    }
    template <size_t I>
    constexpr typename get_type <I, T0, Ts ...> :: value const & get_l (std::integral_constant <size_t, I>) const
    {
        return static_cast <typename get_type <I, T0, Ts ...> :: value const &> (tail.template get_l (std::integral_constant <size_t, I - 1> ()));
    }
    constexpr T0 const & get_l (std::integral_constant <size_t, 0>) const
    {
        return static_cast <T0 const &> (value);
    }

    template <size_t I>
    constexpr typename get_type <I, T0, Ts ...> :: value & get_l (std::integral_constant <size_t, I>)
    {
        return static_cast <typename get_type <I, T0, Ts ...> :: value &> (tail.template get_l (std::integral_constant <size_t, I - 1> ()));
    }
    constexpr T0 & get_l (std::integral_constant <size_t, 0>)
    {
        return static_cast <T0 &> (value);
    }

    template <size_t I>
    constexpr typename get_type <I, T0, Ts ...> :: value const && get (std::integral_constant <size_t, I>) const
    {
        return static_cast <typename get_type <I, T0, Ts ...> :: value const &&> (tail.template get (std::integral_constant <size_t, I - 1> ()));
    }
    constexpr T0 const && get (std::integral_constant <size_t, 0>) const
    {
        return static_cast <T0 const &&> (value);
    }

    template <size_t I>
    constexpr typename get_type <I, T0, Ts ...> :: value && get (std::integral_constant <size_t, I>)
    {
        return static_cast <typename get_type <I, T0, Ts ...> :: value &&> (tail.template get (std::integral_constant <size_t, I - 1> ()));
    }
    constexpr T0 && get (std::integral_constant <size_t, 0>)
    {
        return static_cast <T0 &&> (value);
    }
    
    template <typename STORAGE>
    constexpr void move (STORAGE && t, size_t ind, size_t old)
    {
        if (old != variant_npos)
            this->destroy(old);
        if (ind == 0)
            new (&this->value) T0(std::forward <STORAGE> (t).value);
        else
            this->tail.move(std::forward <STORAGE> (t).tail, ind - 1, (old == variant_npos)? variant_npos : (old - 1));
    }
    template <typename STORAGE>
    constexpr void swap(STORAGE & rhs, size_t & i, size_t & rhs_i)
    {
        if (i == 0 && rhs_i == 0)
            std::swap (this->value, rhs.value);
        else if (i == rhs_i)
        {
            size_t i_ = i - 1, rhs_i_ = rhs_i - 1;
            tail.swap (rhs.tail, i_, rhs_i_);
        }
        else if (i != rhs_i)
        {
            size_t i_ = i, rhs_i_ = rhs_i;
            STORAGE tmp;
            tmp.move(*this, i_, variant_npos);
            i = variant_npos;
            this->move(rhs, rhs_i_, i_);
            i = rhs_i_;
            rhs_i = variant_npos;
            rhs.move(tmp, i_, rhs_i_);
            rhs_i = i_;
        }
    }
    
    template <typename STORAGE>
    constexpr void copy (STORAGE const & t, size_t ind, size_t old)
    {
        if (old != variant_npos)
            this->destroy(old);
        if (ind == 0)
            new (&this->value) T0(std::forward <STORAGE const> (t).value);
        else
            this->tail.copy(std::forward <STORAGE const> (t).tail, ind - 1, (old == variant_npos)? variant_npos : (old - 1));
    }

    template <typename STORAGE>
    constexpr bool comp_me (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value <= a.value);
        return this->tail.comp_me(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_be (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value >= a.value);
        return this->tail.comp_be(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_me (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value <= a.value);
        return this->tail.comp_me(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_be (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value >= a.value);
        return this->tail.comp_be(a.tail, i - 1);
    }

    template <typename STORAGE>
    constexpr bool comp_b (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value > a.value);
        return this->tail.comp_b(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_ne (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value != a.value);
        return this->tail.comp_ne(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_b (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value > a.value);
        return this->tail.comp_b(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_ne (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value != a.value);
        return this->tail.comp_ne(a.tail, i - 1);
    }

    template <typename STORAGE>
    constexpr bool comp_m (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value < a.value);
        return this->tail.comp_m(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_e (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value == a.value);
        return this->tail.comp_e(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_m (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value < a.value);
        return this->tail.comp_m(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_e (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value == a.value);
        return this->tail.comp_e(a.tail, i - 1);
    }
    ~storage() noexcept {};
};

template <bool f, typename T0>
struct storage <f, T0>
{
public:
    union 
    {
        T0 value;
    };
    constexpr storage ()
    {}
    template <typename  T>
    constexpr T & set (std::integral_constant <size_t, 0>, T && t)
    {
        assert ((is_same <T, T0> :: value));
        new (&value) T0(std::forward <T> (t));
        return value;
    }
    template <typename ... Args>
    constexpr typename get_type <0, T0> :: type & set (std::integral_constant <size_t, 0>, Args && ... args)
    {
        new (&value) T0(std::forward <Args> (args) ...);
        return value;
    }
    //constructor
    template <typename  T>
    constexpr storage (std::integral_constant <size_t, 0>, T && t)
    {
        new (&value) T0(std::forward <T> (t));
    }
    template <typename  T, typename ... Args>
    constexpr storage (std::integral_constant <size_t, 0>, std::in_place_type_t <T>, Args && ... args) :
        value(std::forward <Args> (args) ...)
    {}

    template <typename ... Args>
    constexpr storage (std::in_place_index_t <0>, Args && ... args) :
        value(std::forward <Args> (args) ...)
    {}

    //destructor
    constexpr void destroy (size_t I)
    {
        assert (I == 0);
        value.~T0();
    }
    constexpr T0 const & get_l (std::integral_constant <size_t, 0>) const
    {
        return static_cast <T0 const &> (value);
    }

    constexpr T0 & get_l (std::integral_constant <size_t, 0>)
    {
        return static_cast <T0 &> (value);
    }

    constexpr T0 const && get (std::integral_constant <size_t, 0>) const
    {
        return static_cast <T0 const &&> (value);
    }

    constexpr T0 && get (std::integral_constant <size_t, 0>)
    {
        return static_cast <T0 &&> (value);
    }
    
    template <typename STORAGE>
    constexpr void move (STORAGE && t, size_t ind, size_t old)
    {
        if (old != variant_npos)
            this->destroy(old);
        assert (ind == 0);
        new (&this->value) T0(std::forward <STORAGE> (t).value);
    }
    template <typename STORAGE>
    constexpr void swap(STORAGE & rhs, size_t & i, size_t & rhs_i)
    {
        assert (i == 0 && rhs_i == 0);
        std::swap (this->value, rhs.value);
    }
    
    template <typename STORAGE>
    constexpr void copy (STORAGE const & t, size_t ind, size_t old)
    {
        if (old != variant_npos)
            this->destroy(old);
        assert (ind == 0);
        new (&this->value) T0(std::forward <STORAGE const> (t).value);
    }


    template <typename STORAGE>
    constexpr bool comp_me (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value <= a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_me (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value <= a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_be (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value >= a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_be (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value >= a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_b (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value > a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_b (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value > a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_ne (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value != a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_ne (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value != a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_e (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value == a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_e (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value == a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_m (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value < a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_m (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value < a.value);
    }
    ~storage() noexcept {};
};

template <typename T0, typename ... Ts>
struct storage <1, T0, Ts ...>
{
public:
    union 
    {
        T0 value;
        storage <1, Ts ...> tail;
    };
    constexpr storage ()
    {
        new (&value) T0();
    }
    template <typename  T, size_t I>
    constexpr T & set (std::integral_constant <size_t, I>, T && t)
    {
        return tail.template set (std::integral_constant <size_t, I - 1>(), std::forward <T> (t));
    }
    template <typename  T>
    constexpr T & set (std::integral_constant <size_t, 0>, T && t)
    {
        assert ((is_same <T, T0> :: value));
        new (&value) T0(std::forward <T> (t));
        return value;
    }
    template <size_t I, typename ... Args>
    constexpr typename get_type <I, T0, Ts ...> :: type & set (std::integral_constant <size_t, I>, Args && ... args)
    {
        return tail.template set (std::integral_constant <size_t, I - 1>(), std::forward <Args> (args) ...);
    }
    template <typename ... Args>
    constexpr typename get_type <0, T0> :: type & set (std::integral_constant <size_t, 0>, Args && ... args)
    {
        new (&value) T0(std::forward <Args> (args) ...);
        return value;
    }
    //constructor
    template <size_t I, typename  T>
    constexpr storage (std::integral_constant <size_t, I>, T && t) :
        tail(std::integral_constant <size_t, I - 1>(),  std::forward <T> (t))
    {}
    template <typename  T>
    constexpr storage (std::integral_constant <size_t, 0>, T && t) :
        value(std::forward <T> (t))
    {}
    
    template <size_t I, typename  T, typename ... Args>
    constexpr storage (std::integral_constant <size_t, I>, std::in_place_type_t <T>, Args && ... args) :
        tail(std::integral_constant <size_t, I - 1>(), std::in_place_type_t <T> (), std::forward <Args> (args)...)
    {}
    template <typename  T, typename ... Args>
    constexpr storage (std::integral_constant <size_t, 0>, std::in_place_type_t <T>, Args && ... args) :
        value(std::forward <Args> (args) ...)
    {}

    template <size_t I, typename ... Args>
    constexpr storage (std::in_place_index_t <I>, Args && ... args) :
        tail(std::in_place_index_t <I - 1> (), std::forward <Args> (args)...)
    {}
    template <typename ... Args>
    constexpr storage (std::in_place_index_t <0>, Args && ... args) :
        value(std::forward <Args> (args) ...)
    {}
    //destroy
    constexpr void destroy (size_t I)
    {
        if (I == 0)
            value.~T0();
        else
            tail.destroy(I - 1);
    }
    template <size_t I>
    constexpr typename get_type <I, T0, Ts ...> :: value const & get_l (std::integral_constant <size_t, I>) const
    {
        return static_cast <typename get_type <I, T0, Ts ...> :: value const &> (tail.template get_l (std::integral_constant <size_t, I - 1> ()));
    }
    constexpr T0 const & get_l (std::integral_constant <size_t, 0>) const
    {
        return static_cast <T0 const &> (value);
    }

    template <size_t I>
    constexpr typename get_type <I, T0, Ts ...> :: value & get_l (std::integral_constant <size_t, I>)
    {
        return static_cast <typename get_type <I, T0, Ts ...> :: value &> (tail.template get_l (std::integral_constant <size_t, I - 1> ()));
    }
    constexpr T0 & get_l (std::integral_constant <size_t, 0>)
    {
        return static_cast <T0 &> (value);
    }

    template <size_t I>
    constexpr typename get_type <I, T0, Ts ...> :: value const && get (std::integral_constant <size_t, I>) const
    {
        return static_cast <typename get_type <I, T0, Ts ...> :: value const &&> (tail.template get (std::integral_constant <size_t, I - 1> ()));
    }
    constexpr T0 const && get (std::integral_constant <size_t, 0>) const
    {
        return static_cast <T0 const &&> (value);
    }

    template <size_t I>
    constexpr typename get_type <I, T0, Ts ...> :: value && get (std::integral_constant <size_t, I>)
    {
        return static_cast <typename get_type <I, T0, Ts ...> :: value &&> (tail.template get (std::integral_constant <size_t, I - 1> ()));
    }
    constexpr T0 && get (std::integral_constant <size_t, 0>)
    {
        return static_cast <T0 &&> (value);
    }
    
    template <typename STORAGE>
    constexpr void move (STORAGE && t, size_t ind, size_t old)
    {
        if (old != variant_npos)
            this->destroy(old);
        if (ind == 0)
            new (&this->value) T0(std::forward <STORAGE> (t).value);
        else
            this->tail.move(std::forward <STORAGE> (t).tail, ind - 1, (old == variant_npos)? variant_npos : (old - 1));
    }
    template <typename STORAGE>
    constexpr void swap(STORAGE & rhs, size_t & i, size_t & rhs_i)
    {
        if (i == 0 && rhs_i == 0)
            std::swap (this->value, rhs.value);
        else if (i == rhs_i)
        {
            size_t i_ = i - 1, rhs_i_ = rhs_i - 1;
            tail.swap (rhs.tail, i_, rhs_i_);
        }
        else if (i != rhs_i)
        {
            size_t i_ = i, rhs_i_ = rhs_i;
            STORAGE tmp;
            tmp.move(*this, i_, variant_npos);
            i = variant_npos;
            this->move(rhs, rhs_i_, i_);
            i = rhs_i_;
            rhs_i = variant_npos;
            rhs.move(tmp, i_, rhs_i_);
            rhs_i = i_;
        }
    }
    
    template <typename STORAGE>
    constexpr void copy (STORAGE const & t, size_t ind, size_t old)
    {
        if (old != variant_npos)
            this->destroy(old);
        if (ind == 0)
            new (&this->value) T0(std::forward <STORAGE const> (t).value);
        else
            this->tail.copy(std::forward <STORAGE const> (t).tail, ind - 1, (old == variant_npos)? variant_npos : (old - 1));
    }

    template <typename STORAGE>
    constexpr bool comp_me (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value <= a.value);
        return this->tail.comp_me(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_be (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value >= a.value);
        return this->tail.comp_be(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_me (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value <= a.value);
        return this->tail.comp_me(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_be (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value >= a.value);
        return this->tail.comp_be(a.tail, i - 1);
    }

    template <typename STORAGE>
    constexpr bool comp_b (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value > a.value);
        return this->tail.comp_b(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_ne (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value != a.value);
        return this->tail.comp_ne(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_b (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value > a.value);
        return this->tail.comp_b(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_ne (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value != a.value);
        return this->tail.comp_ne(a.tail, i - 1);
    }

    template <typename STORAGE>
    constexpr bool comp_m (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value < a.value);
        return this->tail.comp_m(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_e (STORAGE const & a, size_t i) const
    {
        if (i == 0)
            return (this->value == a.value);
        return this->tail.comp_e(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_m (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value < a.value);
        return this->tail.comp_m(a.tail, i - 1);
    }
    template <typename STORAGE>
    constexpr bool comp_e (STORAGE const && a, size_t i) const
    {
        if (i == 0)
            return (this->value == a.value);
        return this->tail.comp_e(a.tail, i - 1);
    }
    ~storage() noexcept = default;
};

template <typename T0>
struct storage <1, T0>
{
public:
    union 
    {
        T0 value;
    };
    constexpr storage () : 
        value()
    {}
    template <typename  T>
    constexpr T & set (std::integral_constant <size_t, 0>, T && t)
    {
        assert ((is_same <T, T0> :: value));
        new (&value) T0(std::forward <T> (t));
        return value;
    }
    template <typename ... Args>
    constexpr typename get_type <0, T0> :: type & set (std::integral_constant <size_t, 0>, Args && ... args)
    {
        new (&value) T0(std::forward <Args> (args) ...);
        return value;
    }
    //constructor
    template <typename  T>
    constexpr storage (std::integral_constant <size_t, 0>, T && t)
    {
        new (&value) T0(std::forward <T> (t));
    }
    template <typename  T, typename ... Args>
    constexpr storage (std::integral_constant <size_t, 0>, std::in_place_type_t <T>, Args && ... args) :
        value(std::forward <Args> (args) ...)
    {}

    template <typename ... Args>
    constexpr storage (std::in_place_index_t <0>, Args && ... args) :
        value(std::forward <Args> (args) ...)
    {}

    //destructor
    constexpr void destroy (size_t I)
    {
        assert (I == 0);
        value.~T0();
    }
    constexpr T0 const & get_l (std::integral_constant <size_t, 0>) const
    {
        return static_cast <T0 const &> (value);
    }

    constexpr T0 & get_l (std::integral_constant <size_t, 0>)
    {
        return static_cast <T0 &> (value);
    }

    constexpr T0 const && get (std::integral_constant <size_t, 0>) const
    {
        return static_cast <T0 const &&> (value);
    }

    constexpr T0 && get (std::integral_constant <size_t, 0>)
    {
        return static_cast <T0 &&> (value);
    }
    
    template <typename STORAGE>
    constexpr void move (STORAGE && t, size_t ind, size_t old)
    {
        if (old != variant_npos)
            this->destroy(old);
        assert (ind == 0);
        new (&this->value) T0(std::forward <STORAGE> (t).value);
    }
    template <typename STORAGE>
    constexpr void swap(STORAGE & rhs, size_t & i, size_t & rhs_i)
    {
        assert (i == 0 && rhs_i == 0);
        std::swap (this->value, rhs.value);
    }
    
    template <typename STORAGE>
    constexpr void copy (STORAGE const & t, size_t ind, size_t old)
    {
        if (old != variant_npos)
            this->destroy(old);
        assert (ind == 0);
        new (&this->value) T0(std::forward <STORAGE const> (t).value);
    }


    template <typename STORAGE>
    constexpr bool comp_me (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value <= a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_me (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value <= a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_be (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value >= a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_be (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value >= a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_b (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value > a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_b (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value > a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_ne (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value != a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_ne (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value != a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_e (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value == a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_e (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value == a.value);
    }

    template <typename STORAGE>
    constexpr bool comp_m (STORAGE const & a, size_t i) const
    {
        assert (i == 0);
        return (this->value < a.value);
    }
    template <typename STORAGE>
    constexpr bool comp_m (STORAGE const && a, size_t i) const
    {
        assert (i == 0);
        return (this->value < a.value);
    }
    ~storage() noexcept = default;
};


#endif
