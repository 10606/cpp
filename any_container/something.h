
#pragma once
#include <assert.h>
#include <utility>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "any_iterator.h"
constexpr size_t small_storage_size = sizeof(void*);
constexpr size_t small_storage_alignment = alignof(void*);
using small_storage = std::aligned_storage<small_storage_size, small_storage_alignment>::type;

template <typename InnerIterator>
constexpr bool is_small
    = sizeof(InnerIterator) <= small_storage_size
   && alignof(InnerIterator) <= small_storage_alignment
   && std::is_trivially_move_constructible<InnerIterator>::value;
/*
template <typename T, typename category, template <typename> typename container> 
typename container <T> :: iterator const & get_iter(any_iterator <T, category> const & a)
{
    return any_iterator_impl::access <typename container <T> :: iterator> (a.stg);
}
*/


template <typename T, typename category, template <typename> typename container, bool f> 
std::enable_if_t <!is_small <container <T> >, any_const_iterator <T, category> > cend (small_storage const & storage)
{
    any_const_iterator <T, category> ret((* reinterpret_cast <container <T> * const &> (storage)).cend());
    return ret;
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <!is_small <container <T> >, any_const_iterator <T, category> > cbegin (small_storage const & storage)
{
    any_const_iterator <T, category> ret((* reinterpret_cast <container <T> * const &> (storage)).cbegin());
    return ret;
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <!is_small <container <T> >, any_iterator <T, category> > end (small_storage & storage)
{
    any_iterator <T, category> ret((* reinterpret_cast <container <T> * &> (storage)).end());
    return ret;
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <!is_small <container <T> >, any_iterator <T, category> > begin (small_storage & storage)
{
    any_iterator <T, category> ret((* reinterpret_cast <container <T> * &> (storage)).begin());
    return ret;
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <!is_small <container <T> >, any_iterator <T, category> > erase(small_storage & storage, any_const_iterator <T, category> to)
{
    //return to;
    assert ((any_iterator_impl::check <T, category, typename container <T> :: iterator> (to)));
    return any_iterator <T, category> (
        (* reinterpret_cast <container <T> * &> (storage)) 
        .erase (any_iterator_impl::get_iter <T, category, container> (to))
    ); //TODO
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <!is_small <container <T> >, any_iterator <T, category> > insert_r(small_storage & storage, T && a, any_const_iterator <T, category> to)
{
    //return to;
    assert ((any_iterator_impl::check <T, category, typename container <T> :: iterator> (to)));
    return any_iterator <T, category> (
        (* reinterpret_cast <container <T> * &> (storage))
        .insert (any_iterator_impl::get_iter <T, category, container> (to), std::move(a))
    ); //TODO
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <((!is_small <container <T> >) && (std::is_copy_constructible_v <T>)), any_iterator <T, category> > insert(small_storage & storage, T const & a, any_const_iterator <T, category> to)
{
    //return to;
    assert ((any_iterator_impl::check <T, category, typename container <T> :: iterator> (to)));
    return any_iterator <T, category> (
        (* reinterpret_cast <container <T> * &> (storage))
        .insert (any_iterator_impl::get_iter <T, category, container> (to), a)
    ); //TODO
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <!is_small <container <T> >, size_t > size(small_storage const & storage)
{
    return (* reinterpret_cast <container <T> * const &> (storage)).size ();
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <!is_small <container <T> >, bool> empty(small_storage const & storage)
{
    return (* reinterpret_cast <container <T> * const &> (storage)).empty ();
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <!is_small <container <T> >, void> destroy (small_storage & storage)
{
    delete (reinterpret_cast <container <T> * &> (storage));
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <!is_small <container <T> >, void> move (small_storage & to, small_storage & from)
{
    //std::cout << "big move\n";
    new (&to) container <T> * (new container <T> (std::move (* reinterpret_cast <container <T> * &> (from))));
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <((!is_small <container <T> >) && (std::is_copy_constructible_v <T> )), void> copy (small_storage & to, small_storage const & from)
{
    //std::cout << "big copy\n";
    new (&to) container <T> * (new container <T> (* reinterpret_cast <container <T> * const &> (from)));
}





template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, any_const_iterator <T, category> > cend   (small_storage const & storage)
{
    return any_const_iterator <T, category> ((reinterpret_cast <container <T> const &> (storage)).cend());
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, any_const_iterator <T, category> > cbegin   (small_storage const & storage)
{
    return any_const_iterator <T, category> ((reinterpret_cast <container <T> const &> (storage)).cbegin());
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, any_iterator <T, category> > end   (small_storage & storage)
{
    return  any_iterator <T, category> ((reinterpret_cast <container <T> &> (storage)).end());
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, any_iterator <T, category> > begin   (small_storage & storage)
{
    return  any_iterator <T, category> ((reinterpret_cast <container <T> &> (storage)).begin());
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, any_iterator <T, category> > erase  (small_storage & storage, any_const_iterator <T, category> to)
{
    //return to;
    assert ((any_iterator_impl::check <T, category, typename container <T> :: iterator> (to)));
    return any_iterator <T, category> (
        (reinterpret_cast <container <T> &> (storage))
        .erase (any_iterator_impl::get_iter <T, category, container> (to))
    ); //TODO
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, any_iterator <T, category> > insert_r  (small_storage & storage, T && a, any_const_iterator <T, category> to)
{
    //return to;
    assert ((any_iterator_impl::check <T, category, typename container <T> :: iterator> (to)));
    return any_iterator <T, category> (
        (reinterpret_cast <container <T> &> (storage))
        .insert (any_iterator_impl::get_iter <T, category, container> (to), std::move(a))
    ); //TODO
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <((is_small <container <T> >) && (std::is_copy_constructible_v <T>)), any_iterator <T, category> > insert  (small_storage & storage, T const & a,  any_const_iterator <T, category> to)
{
    //return to;
    assert ((any_iterator_impl::check <T, category, typename container <T> :: iterator> (to)));
    return any_iterator <T, category> (
        (reinterpret_cast <container <T> &> (storage))
        .insert (any_iterator_impl::get_iter <T, category, container> (to), a)
    ); //TODO
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, size_t> size  (small_storage const & storage)
{
    return (reinterpret_cast <container <T> const &> (storage)).size ();
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, bool> empty  (small_storage const & storage)
{
    return (reinterpret_cast <container <T> const &> (storage)).empty ();
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, void> destroy   (small_storage & storage)
{
    (reinterpret_cast <container <T> &> (storage)).~container <T> ();
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <is_small <container <T> >, void> move   (small_storage & to, small_storage & from)
{
    //std::cout << "small move\n";
    new (&to) container <T> (std::move (reinterpret_cast <container <T> &> (from)));
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <((is_small <container <T> >) && (std::is_copy_constructible_v <T> )), void> copy   (small_storage & to, small_storage const & from)
{
    //std::cout << "small copy\n";
    new (&to) container <T> (reinterpret_cast <container <T> const &> (from));
}




template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <(!std::is_copy_constructible_v <T>), any_iterator <T, category> > insert(small_storage & storage, T const & a, any_const_iterator <T, category> to)
{
    throw;
}

template <typename T, typename category, template <typename> typename container, bool f > 
 std::enable_if_t <(!std::is_copy_constructible_v <T> ), void> copy   (small_storage & to, small_storage const & from)
{
    throw;
}

