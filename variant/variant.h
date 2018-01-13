#ifndef VARIANT_H
#define VARIANT_H

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
#include "storage.h"
#include "storage_2.h"

template <typename T0, typename ... Ts>
struct variant {
    template <typename = std::enable_if_t <std::is_default_constructible <T0> :: value>>
    constexpr variant () :
        stor(),
        ind(0)
    {
        stor.ind = ind;
    }
    template <class T, class... Args>
    constexpr T& emplace(Args&&... args)
    {
        if (ind != variant_npos)
            stor.destroy(ind);
        stor.ind = variant_npos;
        ind = variant_npos;
        T & ret(stor.set(std::integral_constant <size_t, index_of <T, T0, Ts ...> :: value> (), std::forward <Args> (args) ...));
        stor.ind = index_of <T, T0, Ts...>::value;
        ind = stor.ind;
        return ret;
    }
    template <size_t I, class... Args>
    constexpr typename get_type <I, T0, Ts ...> :: type & emplace(Args&&... args)
    {
        if (ind != variant_npos)
            stor.destroy(ind);
        stor.ind = variant_npos;
        ind = variant_npos;
        typename get_type <I, T0, Ts ...> :: type & ret (stor.set(std::integral_constant <size_t, I> (), std::forward <Args> (args) ...));
        stor.ind = I;
        ind = stor.ind;
        return ret;
    }
    template <typename  T, typename ... Args>
    constexpr variant (std::in_place_type_t <T>, Args && ... args) :
        stor (std::in_place_type_t <T> (), std::forward <Args> (args)...),
        ind(index_of <T, T0, Ts ...> :: value)
    {}
    
    template <size_t I, typename ... Args>
    constexpr variant (std::in_place_index_t <I>, Args && ... args) :
        stor (std::in_place_index_t <I> (), std::forward <Args> (args)...),
        ind(I)
    {}
    template <typename T>
    constexpr variant (variant <T0, Ts ...> const & t) :
        stor(std::forward(t.stor)),
        ind(t.ind)
    {
        static_assert (index_of <T, T0, Ts ...> :: value != -1);
    }
    template <typename T>
    constexpr variant (variant <T0, Ts ...> && t) :
        stor(std::forward(t.stor)),
        ind(t.ind)
    {
        stor.ind = ind;
        static_assert (index_of <T, T0, Ts ...> :: value != -1);
    }
    template <typename T>
    constexpr variant (T && t) :
        stor(std::in_place_index_t <get_F_index <T, T0, Ts ...> :: value> (), std::forward <T &&> (t)),
        ind (static_cast <size_t> (get_F_index <T, T0, Ts ...> :: value))
    {
        stor.ind = ind;
        //static_assert (index_of <T, T0, Ts ...> :: value != -1);
    }
    constexpr void swap( variant& rhs )
    {
        stor.swap(rhs.stor, stor.ind, rhs.stor.ind);
        std::swap(ind, rhs.ind);
    }

    copyable_storage <check_copy <T0, Ts ...> :: value, T0, Ts ...> stor;
    size_t ind;
    constexpr size_t index () const
    {
        return ind;
    }
    constexpr bool valueless_by_exception() const noexcept
    {
        return index() == variant_npos;
    }

    template <class... Types>
    friend constexpr bool operator==(const variant<Types...>& v, const variant<Types...>& w);
    template <class... Types>
    friend constexpr bool operator!=(const variant<Types...>& v, const variant<Types...>& w);
    template <class... Types>
    friend constexpr bool operator<=(const variant<Types...>& v, const variant<Types...>& w);
    template <class... Types>
    friend constexpr bool operator>=(const variant<Types...>& v, const variant<Types...>& w);
    template <class... Types>
    friend constexpr bool operator<(const variant<Types...>& v, const variant<Types...>& w);
    template <class... Types>
    friend constexpr bool operator>(const variant<Types...>& v, const variant<Types...>& w);
};

template <class... Types>
constexpr bool operator==(const variant<Types...>& v, const variant<Types...>& w)
{
    if (v.index() != w.index())
        return false;
    if (v.valueless_by_exception())
        return true;
    return v.stor.comp_e(w.stor, v.index());
}
template <class... Types>
constexpr bool operator!=(const variant<Types...>& v, const variant<Types...>& w)
{
    if (v.index() != w.index())
        return true;
    if (v.valueless_by_exception())
        return false;
    return v.stor.comp_ne(w.stor, v.index());
}
template <class... Types>
constexpr bool operator<(const variant<Types...>& v, const variant<Types...>& w)
{
    if (w.valueless_by_exception())
        return false;
    if (v.valueless_by_exception())
        return true;
    if (v.index() < w.index())
        return true;
    if (v.index() > w.index())
        return false;
    return v.stor.comp_m(w.stor, v.index());
}
template <class... Types>
constexpr bool operator>(const variant<Types...>& v, const variant<Types...>& w)
{
    if (v.valueless_by_exception())
        return false;
    if (w.valueless_by_exception())
        return true;
    if (v.index() > w.index())
        return true;
    if (v.index() < w.index())
        return false;
    return v.stor.comp_b(w.stor, v.index());
}
template <class... Types>
constexpr bool operator<=(const variant<Types...>& v, const variant<Types...>& w)
{
    if (v.valueless_by_exception())
        return true;
    if (w.valueless_by_exception())
        return false;
    if (v.index() < w.index())
        return true;
    if (v.index() > w.index())
        return false;
    return v.stor.comp_me(w.stor, v.index());
}
template <class... Types>
constexpr bool operator>=(const variant<Types...>& v, const variant<Types...>& w)
{
    if (w.valueless_by_exception())
        return true;
    if (v.valueless_by_exception())
        return false;
    if (v.index() > w.index())
        return true;
    if (v.index() < w.index())
        return false;
    return v.stor.comp_be(w.stor, v.index());
}

template <class... Types>
void swap(variant<Types...>& lhs, variant<Types...>& rhs)
{
    lhs.swap(rhs);
}
template <class T>
struct variant_size;

template <class... Types>
struct variant_size<variant<Types...>>
: std::integral_constant<size_t, sizeof...(Types)> { };

template <class ... Types> 
struct variant_size<const variant <Types ...>>
: std::integral_constant<size_t, sizeof...(Types)> { };

template <class ... Types> 
struct variant_size<volatile variant <Types ...>>
: std::integral_constant<size_t, sizeof...(Types)> { };

template <class ... Types> 
struct variant_size<const volatile variant <Types ...>>
: std::integral_constant<size_t, sizeof...(Types)> { };

template <std::size_t I, class T>
struct variant_alternative; 

template <size_t I, class... Types>
struct variant_alternative <I, variant<Types...>>
{
    typedef typename get_type <I, Types ...> :: type type;
};

template <size_t I, class... Types>
struct variant_alternative <I, const variant<Types...>>
{
    typedef typename get_type <I, Types ...> :: type type;
};

template <size_t I, class... Types>
struct variant_alternative <I, volatile variant<Types...>>
{
    typedef typename get_type <I, Types ...> :: type type;
};

template <size_t I, class... Types>
struct variant_alternative <I, const volatile variant<Types...>>
{
    typedef typename get_type <I, Types ...> :: type type;
};

template <class T, class... Types>
constexpr bool holds_alternative(const variant<Types...>& v) noexcept
{
    return (index_of <T, Types ...> :: value == static_cast <int> (v.index()));
}

struct bad_variant_access : public std::exception
{
	bad_variant_access() noexcept
		: exception()
		, reason("bad_variant_access")
	{}

	bad_variant_access(const char* why) noexcept
		: exception()
		, reason(why)
	{}

	const char* what() const noexcept override
	{
		return reason;
	}

	~bad_variant_access() override = default;
private:
	char const* reason;
};


template <size_t I, class... Types>
constexpr typename variant_alternative <I, variant<Types...>> :: type& 
  get(variant<Types...>& v)
{
    if (I != v.index())
		throw bad_variant_access();
    return (v.stor.template get_l (std::integral_constant <size_t, I> ()));
}

template <size_t I, class... Types>
constexpr typename variant_alternative <I, variant<Types...>> :: type const & 
  get(variant<Types...> const & v)
{
    if (I != v.index())
		throw bad_variant_access();
    return (v.stor.template get_l (std::integral_constant <size_t, I> ()));
}

template <size_t I, class... Types>
constexpr typename variant_alternative <I, variant<Types...>> :: type&& 
  get(variant<Types...>&& v)
{
    if (I != v.index())
		throw bad_variant_access();
    return (v.stor.template get (std::integral_constant <size_t, I> ()));
}

template <size_t I, class... Types>
constexpr typename variant_alternative <I, variant<Types...>> :: type const && 
  get(variant<Types...> const && v)
{
    if (I != v.index())
		throw bad_variant_access();
    return (v.stor.template get (std::integral_constant <size_t, I> ()));
}


template <class T, class... Types>
constexpr T& get(variant<Types...>& v)
{
    if (index_of <T, Types ...> :: value != v.index())
		throw bad_variant_access();
    return v.stor.template get_l (std::integral_constant <size_t, index_of <T, Types ...> :: value> ());
}

template <class T, class... Types>
constexpr T const & get(variant<Types...> const & v)
{
    if (index_of <T, Types ...> :: value != v.index())
		throw bad_variant_access();
    return v.stor.template get_l (std::integral_constant <size_t, index_of <T, Types ...> :: value> ());
}

template <class T, class... Types>
constexpr T&& get(variant<Types...>&& v)
{
    if (index_of <T, Types ...> :: value != v.index())
		throw bad_variant_access();
    return v.stor.template get_l (std::integral_constant <size_t, index_of <T, Types ...> :: value> ());
}

template <class T, class... Types>
constexpr T const && get(variant<Types...> const && v)
{
    if (index_of <T, Types ...> :: value != v.index())
		throw bad_variant_access();
    return v.stor.template get_l (std::integral_constant <size_t, index_of <T, Types ...> :: value> ());
}

template <size_t I, typename ... Types>
constexpr decltype(auto) get_if(variant<Types...>* pv) noexcept
{
	return ((!pv || (pv->index() != I)) ? nullptr : &get<I>(*pv));
}

template <size_t I, typename ... Types>
constexpr decltype(auto) get_if(variant<Types...> const* pv) noexcept
{
	return ((!pv || (pv->index() != I)) ? nullptr : &get<I>(*pv));
}

template <typename T, typename ... Types>
constexpr decltype(auto) get_if(variant<Types...>* pv) noexcept
{
	return ((!pv || (pv->index() != index_of <T, Types ...> :: value)) ? nullptr : &get<T>(*pv));
}

template <typename T, typename ... Types>
constexpr decltype(auto) get_if(variant<Types...> const* pv) noexcept
{
	return ((!pv || (pv->index() != index_of <T, Types ...> :: value)) ? nullptr : &get<T>(*pv));
}




template <typename T, size_t ... index>
struct func_stor
{
	constexpr const T & get_func() const
	{
		return data;
	}
	T data;
};

template <typename T, size_t index0, size_t ... indexes>
struct func_stor <T, index0, indexes ...>
{
	template <typename ... size_ts>
	constexpr const T & get_func (size_t first_ind, size_ts ... other_inds) const
	{
		return data_arr[first_ind].get_func(other_inds ...);
	}

	func_stor <T, indexes ...> data_arr[index0];
};

template <typename ... Ts>
struct list_variants;

template <typename func_stor_type, typename variants, typename index_seq>
struct make_func_stor;

template 
<
    typename ret, 
    typename vis, 
    typename ... vars, 
    size_t index0, 
    size_t ... indexes, 
    size_t ... inds
>
struct make_func_stor 
        <
            func_stor <ret (*) (vis, vars ...), index0, indexes ...>, 
            list_variants <vars ...>, 
            std::index_sequence <inds ...>
        >
{
	using func_stor_type = func_stor <ret (*) (vis, vars ...), index0, indexes ...>;
	static constexpr func_stor_type build()
	{
		return build_array
        (
            std::make_index_sequence
            <
                variant_size
                <
                    typename get_type
                    <
                        sizeof ... (inds), 
                        typename std::remove_reference <vars> :: type ...
                    > :: type
                > :: value
            >()
        );
	}
	template <size_t ... var_inds>
	static constexpr func_stor_type build_array(std::index_sequence <var_inds ...>)
	{
		return func_stor_type 
        {
            make_func_stor 
            <
                func_stor <ret (*) (vis, vars ...), indexes ...>, 
                list_variants <vars ...>, 
                std::index_sequence <inds ..., var_inds> 
            > :: build() ... 
        };
	}
};

template 
<
    typename ret, 
    typename vis, 
    typename ... vars, 
    size_t ... inds
>
struct make_func_stor 
<
    func_stor <ret (*) (vis, vars ...)>, 
    list_variants <vars ...>, 
    std::index_sequence <inds ...>
>
{
	static constexpr decltype(auto) exec(vis visitor, vars ... variants)
	{
		return std::forward <vis> (visitor) (get <inds> (std::forward <vars> (variants)) ...);
	}
	static constexpr func_stor <ret (*) (vis, vars ...)> build()
	{
		return func_stor <ret (*) (vis, vars ...)> {& exec };
	}
};

template <typename Visitor, typename ... Variants>
constexpr decltype(auto) visit (Visitor && vis, Variants && ... vars)
{
	if ((vars.valueless_by_exception() || ...))
	{
		throw bad_variant_access();
	}
	constexpr auto exec = make_func_stor 
                            <
                                func_stor <
                                    decltype
                                    (
                                        std::forward <Visitor> (vis) 
                                        (get <0> (std::forward <Variants> (vars)) ...)
                                    ) (*) (Visitor &&, Variants && ...),
                                    variant_size <typename std::remove_reference <Variants> :: type> :: value ...
                                >, 
                                list_variants <Variants && ...>,
                                std::index_sequence <>
                            > :: build();
	return exec.get_func(vars.index() ...) 
            (
                std::forward <Visitor> (vis), 
                std::forward <Variants> (vars) ...
            );
}

#endif
