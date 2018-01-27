#pragma once
#include <type_traits>
#include <utility>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "any_iterator.h"
#include "something.h"

template <typename T>
struct null_container
{};



template <typename T, typename category > 
const any_iterator <const T, category> null_cend   (small_storage const & storage)
{
    throw;
}

template <typename T, typename category > 
const any_iterator <const T, category> null_cbegin   (small_storage const & storage)
{
    throw;
}

template <typename T, typename category > 
any_iterator <T, category> null_end   (small_storage & storage)
{
    throw;
}

template <typename T, typename category > 
any_iterator <T, category> null_begin   (small_storage & storage)
{
    throw;
}

template <typename T, typename category > 
any_iterator <T, category> null_erase  (small_storage & storage, any_iterator <T, category> to)
{
    throw;
}

template <typename T, typename category > 
any_iterator <T, category> null_insert_r  (small_storage & storage, T && a, any_iterator <T, category> to)
{
    throw;
}

template <typename T, typename category > 
any_iterator <T, category> null_insert  (small_storage & storage, T const & a, any_iterator <T, category> to)
{
    throw;
}

template <typename T, typename category > 
size_t null_size  (small_storage const & storage)
{
    return 0;
}

template <typename T, typename category > 
bool null_empty  (small_storage const & storage)
{
    return 1;
}

template <typename T, typename category > 
void null_destroy   (small_storage & storage)
{
}

template <typename T, typename category > 
void null_move   (small_storage & to, small_storage & from)
{
}

template <typename T, typename category > 
void null_copy   (small_storage & to, small_storage const & from)
{
}

template <typename T, typename category> 
struct func_stor
{
    using cend__t = const any_iterator <const T, category> (*) (small_storage const & storage);
    using cbegin__t = const any_iterator <const T, category> (*) (small_storage const & storage);
    using end__t = any_iterator <T, category> (*) (small_storage & storage);
    using begin__t = any_iterator <T, category> (*) (small_storage & storage);
    using erase__t = any_iterator <T, category> (*) (small_storage & storage, any_iterator <T, category> to);
    using insert__t = any_iterator <T, category> (*) (small_storage & storage, T const & a, any_iterator <T, category> to);
    using insert_r__t = any_iterator <T, category> (*) (small_storage & storage, T && a, any_iterator <T, category> to);
    using size__t = size_t (*) (small_storage const & storage);
    using empty__t = bool (*) (small_storage const & storage);
    using destroy__t = void (*) (small_storage & storage);
    using move__t = void (*) (small_storage & to, small_storage & from);
    using copy__t = void (*) (small_storage & to, small_storage const & from);

    cend__t cend_e;
    cbegin__t cbegin_e;
    end__t end_e;
    begin__t begin_e;
    erase__t erase_e;
    insert__t insert_e;
    insert_r__t insert_re;
    size__t size_e;
    empty__t empty_e;
    destroy__t destroy_e;
    move__t move_e;
    copy__t copy_e;

    constexpr func_stor (
        cend__t cend_n,
        cbegin__t cbegin_n,
        end__t end_n,
        begin__t begin_n,
        erase__t erase_n,
        insert__t insert_n,
        insert_r__t insert_rn,
        size__t size_n,
        empty__t empty_n,
        destroy__t destroy_n,
        move__t move_n,
        copy__t copy_n

    ) :
        cend_e(cend_n),
        cbegin_e(cbegin_n),
        end_e(end_n),
        begin_e(begin_n),
        erase_e(erase_n),
        insert_e(insert_n),
        insert_re(insert_rn),
        size_e(size_n),
        empty_e(empty_n),
        destroy_e(destroy_n),
        move_e(move_n),
        copy_e(copy_n)
    {}
};

template <typename T, typename category, template <typename> typename container> 
func_stor <T, category> const * get_func_type ()
{
    static constexpr func_stor <T, category> res (
        &cend <T, category, container, is_small <container <T>> >,
        &cbegin <T, category, container, is_small <container <T>> >,
        &end <T, category, container, is_small <container <T>> >,
        &begin <T, category, container, is_small <container <T>> >,
        &erase <T, category, container, is_small <container <T>> >,
        &insert <T, category, container, is_small <container <T>> >,
        &insert_r <T, category, container, is_small <container <T>> >,
        &size <T, category, container, is_small <container <T>> >,
        &empty <T, category, container, is_small <container <T>> >,
        &destroy <T, category, container, is_small <container <T>> >,
        &move <T, category, container, is_small <container <T>> >,
        &copy <T, category, container, is_small <container <T>> >
        );
    return &res;
}

template <typename T, typename category> 
func_stor <T, category> const * get_null_func_type ()
{
    static constexpr func_stor <T, category> res (
        &null_cend <T, category>,
        &null_cbegin <T, category>,
        &null_end <T, category>,
        &null_begin <T, category>,
        &null_erase <T, category>,
        &null_insert <T, category>,
        &null_insert_r <T, category>,
        &null_size <T, category>,
        &null_empty <T, category>,
        &null_destroy <T, category>,
        &null_move <T, category>,
        &null_copy <T, category>
        );
    return &res;
}

template <typename T, template <typename> typename container> //TODO
std::enable_if_t <((!is_small <container <T> >) && (std::is_copy_constructible_v <T> )) > make (small_storage & storage, container <T> const & a)
{
    //std::cout << "big make const &\n";
    new (&storage)  container <T> *(new container <T> (a));
}
template <typename T, template <typename> typename container> //TODO
std::enable_if_t <((is_small <container <T> >) && (std::is_copy_constructible_v <T> )) > make (small_storage & storage, container <T> const & a)
{
    //std::cout << "small make const &\n";
    new (&storage)  container <T> (a);
}
template <typename T, template <typename> typename container> //TODO
std::enable_if_t <!std::is_copy_constructible_v <T> > make (small_storage & storage, container <T> const & a)
{
    throw;
}

template <typename T, template <typename> typename container> //TODO
std::enable_if_t <!is_small <container <T> > > make_r (small_storage & storage, container <T> && a)
{
    //std::cout << "big make &&\n";
    new (&storage)  container <T> *(new container <T> (std::move(a)));
}
template <typename T, template <typename> typename container> //TODO
std::enable_if_t <is_small <container <T> > > make_r (small_storage & storage, container <T> && a)
{
    //std::cout << "small make &&\n";
    new (&storage)  container <T> (std::move(a));
}

template <typename T, typename category>
struct any_container
{
    template <typename category2>
    any_container(any_container<T, category2> const& other,
                 typename std::enable_if<
                     std::is_convertible<category2*, category*>::value
                 >::type* = nullptr)
        : ops(other.ops)
    {
        ops->copy_e(storage, other.storage);
    }

    template <typename category2>
    any_container(any_container<T, category2> && other,
                 typename std::enable_if<
                     std::is_convertible<category2*, category*>::value
                 >::type* = nullptr)
        : ops(other.ops)
    {
        ops->move_e(storage, other.storage);
        other.ops = get_null_func_type<T, category>();
    }
    any_container ()
    {
        //storage = nullptr;
        ops = get_null_func_type <T, category> ();
    }
    template <template <typename> typename container> //TODO
    any_container (container <T> const & a)
    {
        make <T, container> (storage, a);
        //new (&storage)  container <T> *(new container <T> (a));
        //storage = new container <T> (a);
        ops = get_func_type <T, category, container> ();
    }
    template <template <typename> typename container> //TODO
    any_container (container <T> && a)
    {
        make_r <T, container> (storage, std::move(a));
        //new (&storage)  container <T> *(new container <T> (std::move(a)));
        //storage = new container <T> (std::move(a));
        ops = get_func_type <T, category, container> ();
    }
    /*
    template <template <typename> typename container> //TODO
    any_container (container <T> & a)
    {
        new (&storage)  container <T> *(new container <T> (a));
        //storage = new container <T> (a);
        ops = get_func_type <T, category, container> ();
    }
    */
    any_container (any_container && a) :
        ops (a.ops)
    {
        ops->move_e(storage, a.storage);
        a.ops->destroy_e(a.storage);
        a.ops = get_null_func_type <T, category> ();
    }
    any_container (any_container const & a) :
        ops (a.ops)
    {
        ops->copy_e(storage, a.storage);
    }
    void swap (any_container & rhs)
    {
        std::swap (storage, rhs.storage);
        std::swap (ops, rhs.ops);
    }
    any_container & operator = (any_container a)
    {
        swap(a);
        return *this;
    }
    ~any_container ()
    {
        ops->destroy_e(storage);
    }
    bool empty () const
    {
        return ops->empty_e(storage);
    }
    size_t size () const 
    {
        return ops->size_e(storage);
    }
    const any_iterator <const T, category> cbegin () const 
    {
        return ops->cbegin_e(storage);
    }
    const any_iterator <const T, category> cend () const 
    {
        return ops->cend_e(storage);
    }
    any_iterator <T, category> begin () 
    {
        return ops->begin_e(storage);
    }
    any_iterator <T, category> end () 
    {
        return ops->end_e(storage);
    }
    any_iterator <T, category> erase (any_iterator <T, category> to)
    {
        return ops->erase_e (storage, to);
    }
    any_iterator <T, category> insert(any_iterator <T, category> to, T const & a)
    {
        return ops->insert_e (storage, std::move(a), to);
    }
    any_iterator <T, category> insert(any_iterator <T, category> to, T && a)
    {
        return ops->insert_re (storage, std::move(a), to);
    }

    small_storage storage;
    func_stor <T, category> const * ops;
};
