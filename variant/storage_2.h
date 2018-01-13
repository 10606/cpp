
#ifndef STORAGE_2_H
#define STORAGE_2_H

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

template <bool f, typename T0, typename ... Ts>
struct destruct_storage : storage <f, T0, Ts ...>
{
    size_t ind;
    constexpr destruct_storage ():
        ind(variant_npos)
    {}
    
    template <typename  T>
    constexpr destruct_storage (T && t) :
        storage <f, T0, Ts ...> (std::integral_constant <size_t, index_of <T, T0, Ts ...> :: value>(), std::forward <T> (t)),
        ind(index_of <T, T0, Ts...> :: value)
    {} 
    template <typename  T, typename ... Args>
    constexpr destruct_storage (std::in_place_type_t <T>, Args && ... args) :
        storage <f, T0, Ts ...> (std::integral_constant <size_t, index_of <T, T0, Ts ...> :: value>(), std::in_place_type_t <T> (), std::forward <Args> (args)...),
        ind(index_of <T, T0, Ts ...> :: value)
    {}
    
    template <size_t I, typename ... Args>
    constexpr destruct_storage (std::in_place_index_t <I>, Args && ... args) :
        storage <f, T0, Ts...> (std::in_place_index_t <I> (), std::forward <Args> (args)...),
        ind(I)
    {}
    ~destruct_storage()
    {
        this->destroy(ind);
    }
};

template <typename T0, typename ... Ts>
struct destruct_storage <1, T0, Ts ...> : storage <1, T0, Ts ...>
{
    size_t ind;
    constexpr destruct_storage () : 
        ind(variant_npos)
    {}
    
    template <typename  T>
    constexpr destruct_storage (T && t) :
        storage <1, T0, Ts ...> (std::integral_constant <size_t, index_of <T, T0, Ts ...> :: value>(), std::forward <T> (t)),
        ind(index_of <T, T0, Ts...> :: value)
    {} 
    template <typename  T, typename ... Args>
    constexpr destruct_storage (std::in_place_type_t <T>, Args && ... args) :
        storage <1, T0, Ts ...> (std::integral_constant <size_t, index_of <T, T0, Ts ...> :: value>(), std::in_place_type_t <T> (), std::forward <Args> (args)...),
        ind(index_of <T, T0, Ts ...> :: value)
    {}
    
    template <size_t I, typename ... Args>
    constexpr destruct_storage (std::in_place_index_t <I>, Args && ... args) :
        storage <1, T0, Ts...> (std::in_place_index_t <I> (), std::forward <Args> (args)...),
        ind(I)
    {}
};

template <bool f, typename T0, typename ... Ts>
struct movable_storage : destruct_storage <check_destruct <T0, Ts ...> :: value, T0, Ts ...>
{
    constexpr movable_storage ()
    {}
    
    template <typename  T>
    constexpr movable_storage (T && t) :
        destruct_storage <check_destruct <T0, Ts...> :: value, T0, Ts ...> (std::forward <T> (t))
    {}
    template <typename  T, typename ... Args>
    constexpr movable_storage (std::in_place_type_t <T>, Args && ... args) :
        destruct_storage <check_destruct <T0, Ts...> :: value, T0, Ts ...> (std::in_place_type_t <T> (), std::forward <Args> (args)...)
    {}
    
    template <size_t I, typename ... Args>
    constexpr movable_storage (std::in_place_index_t <I>, Args && ... args) :
        destruct_storage <check_destruct <T0, Ts...> :: value, T0, Ts...> (std::in_place_index_t <I> (), std::forward <Args> (args)...)
    {}
    /*
    ~movable_storage ()
    {
        this->~destruct_storage <check_destruct <T0, Ts...> :: value, T0, Ts...>();
    }
    */
};

template <typename T0, typename ... Ts>
struct movable_storage <1, T0, Ts ...> : destruct_storage <check_destruct <T0, Ts ...> :: value, T0, Ts ...>
{
public:
    constexpr movable_storage ()
    {}
    
    constexpr movable_storage (movable_storage && rhs)
    {
        this->ind = rhs.ind;
        this->move(std::forward <movable_storage> (rhs), rhs.ind, variant_npos);
    }
    template <typename  T>
    constexpr movable_storage (T && t) :
        destruct_storage <check_destruct <T0, Ts ...> :: value, T0, Ts ...> (std::forward <T> (t))
    {} 
    template <typename  T, typename ... Args>
    constexpr movable_storage (std::in_place_type_t <T>, Args && ... args) :
        destruct_storage <check_destruct <T0, Ts...> :: value, T0, Ts ...> (std::in_place_type_t <T> (), std::forward <Args> (args)...)
    {}
    
    template <size_t I, typename ... Args>
    constexpr movable_storage (std::in_place_index_t <I>, Args && ... args) :
        destruct_storage <check_destruct <T0, Ts...> :: value, T0, Ts...> (std::in_place_index_t <I> (), std::forward <Args> (args)...)
    {}
    constexpr movable_storage & operator = (movable_storage && rhs)
    {
        swap(rhs, this->ind, rhs.ind);
        return *this;
    }
    /*
    ~movable_storage ()
    {
        this->~destruct_storage <check_destruct <T0, Ts...> :: value, T0, Ts...>();
    }
    */
};

template <bool f, typename T0, typename ... Ts>
struct copyable_storage : movable_storage <check_move <T0, Ts ...> :: value, T0, Ts ...>
{
    constexpr copyable_storage ()
    {}

    template <typename T>
    constexpr copyable_storage (T && t) :
        movable_storage <check_move <T0, Ts ...> :: value, T0, Ts ...> (std::forward <T> (t))
    {}
    template <typename  T, typename ... Args>
    constexpr copyable_storage (std::in_place_type_t <T>, Args && ... args) :
        movable_storage <check_move <T0, Ts...> :: value, T0, Ts ...> (std::in_place_type_t <T> (), std::forward <Args> (args)...)
    {}
    
    template <size_t I, typename ... Args>
    constexpr copyable_storage (std::in_place_index_t <I>, Args && ... args) :
        movable_storage <check_move <T0, Ts...> :: value, T0, Ts...> (std::in_place_index_t <I> (), std::forward <Args> (args)...)
    {}
    /*
    copyable_storage (copyable_storage && rhs) //TODO
    {
        this->ind = rhs.ind;
        this->move(std::forward <copyable_storage> (rhs), rhs.ind, variant_npos);
    }
    copyable_storage (copyable_storage const && rhs)
    {
        this->ind = rhs.ind;
        this->move(std::forward <copyable_storage const> (rhs), rhs.ind, variant_npos);
    }
    constexpr copyable_storage & operator = (copyable_storage rhs)
    {
        swap(rhs, this->ind, rhs.ind);
        return *this;
    }
    */
    /*
    ~copyable_storage()
    {
        std::cout << "~storage\n";
        this->~movable_storage <check_move <T0, Ts ...> :: value, T0, Ts ...> ();
    }
    */
};

template <typename T0, typename ... Ts>
struct copyable_storage <1, T0, Ts ...> : movable_storage <1, T0, Ts ...>
{
public:
    constexpr copyable_storage ()
    {}

    template <typename T>
    constexpr copyable_storage (T && t) :
        movable_storage <1, T0, Ts ...> (std::forward <T> (t))
    {} 
    template <typename  T, typename ... Args>
    constexpr copyable_storage (std::in_place_type_t <T>, Args && ... args) :
        movable_storage <1, T0, Ts ...> (std::in_place_type_t <T> (), std::forward <Args> (args)...)
    {}
    
    template <size_t I, typename ... Args>
    constexpr copyable_storage (std::in_place_index_t <I>, Args && ... args) :
        movable_storage <1, T0, Ts...> (std::in_place_index_t <I> (), std::forward <Args> (args)...)
    {}
    constexpr copyable_storage (copyable_storage && rhs)
    {
        this->ind = rhs.ind;
        this->copy(std::forward <copyable_storage const &> (rhs), rhs.ind, variant_npos);
    }
    constexpr copyable_storage (copyable_storage const && rhs)
    {
        this->ind = rhs.ind;
        this->copy(std::forward <copyable_storage const &> (rhs), rhs.ind, variant_npos);
    }
    constexpr copyable_storage (copyable_storage & rhs)
    {
        this->ind = rhs.ind;
        this->copy(std::forward <copyable_storage const &> (rhs), rhs.ind, variant_npos);
    }
    constexpr copyable_storage (copyable_storage const & rhs)
    {
        this->ind = rhs.ind;
        this->copy(std::forward <copyable_storage const &> (rhs), rhs.ind, variant_npos);
    }
    constexpr copyable_storage & operator = (copyable_storage rhs)
    {
        this->swap(rhs, this->ind, rhs.ind);
        return *this;
    }
    /*
    ~copyable_storage()
    {
        std::cout << "~storage\n";
        this->~movable_storage <1, T0, Ts ...> ();
    }
    */
};
#endif
