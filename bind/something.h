#include <utility>
#include <tuple>
#ifndef SOMETHING_H
#define SOMETHING_H

template <int ... values>
struct integer_sequence
{};

template <int N>
struct placeholder
{};

constexpr placeholder <1> _1;
constexpr placeholder <2> _2;
constexpr placeholder <3> _3;
constexpr placeholder <4> _4;
constexpr placeholder <5> _5;

template <bool calls, typename F, typename ... As>
struct bind_t;

template <typename seq, int arg>
struct add;

template <int ... values, int arg>
struct add <integer_sequence <values ...>, arg>
{
    typedef integer_sequence <values ..., arg> type;
};

template <int N>
struct make_integer_sequence
{
    typedef typename add <typename make_integer_sequence <N - 1> :: type, N - 1> :: type type;
};

template <>
struct make_integer_sequence <0>
{
    typedef integer_sequence <> type;
};

template <typename seq1, typename seq2>
struct merge;

template <int ... seq1, int ... seq2>
struct merge <integer_sequence <seq1 ...>, integer_sequence <seq2 ...> >
{
    typedef integer_sequence <seq1 ..., seq2 ...> value;
};

template <typename ... tail>
struct make_IS_from_list;

template <typename T>
struct make_IS_from_type {
    typedef integer_sequence <> value;
};

template <int N>
struct make_IS_from_type <placeholder <N> >
{
    typedef integer_sequence <N> value;
};

template <int N>
struct make_IS_from_type <placeholder <N> &&>
{
    typedef integer_sequence <N> value;
};

template <int N>
struct make_IS_from_type <placeholder <N> const &>
{
    typedef integer_sequence <N> value;
};

template <bool calls, typename F, typename ... As> 
struct make_IS_from_type <bind_t <calls, F, As ...> >
{
    typedef typename make_IS_from_list <As ...> :: value value;
};

template <bool calls, typename F, typename ... As>
struct make_IS_from_type <bind_t <calls, F, As ...> &&>
{
    typedef typename make_IS_from_list <As ...> :: value value;
};

template <bool calls, typename F, typename ... As>
struct make_IS_from_type <bind_t <calls, F, As ...> const &>
{
    typedef typename make_IS_from_list <As ...> :: value value;
};

template <typename T, typename ... tail>
struct make_IS_from_list <T, tail ...>
{
    typedef typename merge <typename make_IS_from_type <T> :: value, typename make_IS_from_list <tail ...> :: value> :: value value;
};

template <>
struct make_IS_from_list <>
{
    typedef integer_sequence <> value;
};

template <int x, typename seq>
struct count_x_in_IS;

template <int x, int s1, int ... seq>
struct count_x_in_IS <x, integer_sequence <s1, seq ...> >
{
    static const int value = static_cast <int> (x == s1) + count_x_in_IS <x, integer_sequence <seq ...> > :: value;
};

template <int x>
struct count_x_in_IS <x, integer_sequence <> >
{
    static const int value = 0;
};

template <int x, typename seq>
struct need_forward
{
    static const bool value = count_x_in_IS <x, seq> :: value <= 1;
};

template <typename T, bool f>
struct get_type
{
    typedef T & value;
};

template <typename T>
struct get_type<T, 1>
{
    typedef T && value;
};

template <typename T>
struct remove_array
{
    typedef T value;
};

template <typename T, int N>
struct remove_array <T (&) [N]>
{
    typedef T * value;
};

template <bool f, typename A>
struct ret_type
{
    typedef A & value;
};

template <typename A>
struct ret_type <0, A>
{
    typedef typename std::remove_reference <A> :: type && value;
};

template <typename F>
struct get_func_type 
{
    typedef std::remove_reference_t <F> value;
};

template <typename T, typename ... Args>
struct get_func_type <T (&) (Args ...)> 
{
    typedef T (* value) (Args ...);
};

#endif
