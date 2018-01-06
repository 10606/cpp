#include <utility>
#include <tuple>
#include "something.h"
#ifndef BINDF_H
#define BINDF_H

template <bool calls, typename A>
struct G
{
    G(A && _a) : 
        a(std::forward <A> (_a)) 
    {}

    template <typename ... Bs>
    typename ret_type <calls, A> :: value operator () (Bs && ...)
    {
        //return static_cast <decltype(a) &&> (a);  //TODO call_once_bind
        //return static_cast <A &> (a); //TODO bind
        return static_cast <typename ret_type <calls, A> :: value> (a);
    }
private:
    typename std::remove_reference <A> :: type a;
};

template <bool calls>
struct G <calls, placeholder <1> &&>
{
    G(placeholder <1> &&) 
    {}

    G(placeholder <1> const &) 
    {}

    template <typename B1, typename ... Bs>
    decltype(auto) operator () (B1 && b1, Bs && ...)
    {
        return std::forward <B1> (b1);
    }
};

template <bool calls>
struct G <calls, placeholder <1> >
{
    G(placeholder <1> &&) 
    {}

    G(placeholder <1> const &) 
    {}

    template <typename B1, typename ... Bs>
    decltype(auto) operator () (B1 && b1, Bs && ...)
    {
        return std::forward <B1> (b1);
    }
};

template <bool calls>
struct G <calls, placeholder <1> const &>
{
    G(placeholder <1> &&) 
    {}

    G(placeholder <1> const &) 
    {}

    template <typename B1, typename ... Bs>
    decltype(auto) operator () (B1 && b1, Bs && ...)
    {
        return std::forward <B1> (b1);
    }
};

template <bool calls, int N>
struct G <calls, placeholder <N> &&>
{
    G(placeholder <N> &&)
    {}

    G(placeholder <N> const &)
    {}

    template <typename B, typename ... Bs>
    decltype(auto) operator () (B && b, Bs && ... bs)
    {
        G <calls, placeholder <N - 1> > next ( (placeholder <N - 1> ()) );
        return next(std::forward <Bs> (bs) ...);
    }
};

template <bool calls, int N>
struct G <calls, placeholder <N> >
{
    G(placeholder <N> &&)
    {}

    G(placeholder <N> const &)
    {}

    template <typename B, typename ... Bs>
    decltype(auto) operator () (B && b, Bs && ... bs)
    {
        G <calls, placeholder <N - 1> > next ( (placeholder <N - 1> ()) );
        return next(std::forward <Bs> (bs) ...);
    }
};

template <bool calls, int N>
struct G <calls, placeholder <N> const &>
{
    G(placeholder <N> &&)
    {}

    G(placeholder <N> const &)
    {}

    template <typename B, typename ... Bs>
    decltype(auto) operator () (B && b, Bs && ... bs)
    {
        G <calls, placeholder <N - 1> > next ( (placeholder <N - 1> ()) );
        return next(std::forward <Bs> (bs) ...);
    }
};

template <bool calls, bool calls_, typename F, typename ... Arg> 
struct G <calls, bind_t <calls_, F, Arg ...> >
{
    G(bind_t <calls_, F, Arg ...> && _a) : 
        a(std::move(_a)) 
    {}

    template <typename ... Bs>
    decltype(auto) operator () (Bs && ... bs)
    {
        return a(std::forward <Bs> (bs) ...);
    }
private:
    bind_t <calls_, F, Arg ...> a;
};

template <bool calls, bool calls_, typename F, typename ... Arg>
struct G <calls, bind_t <calls_, F, Arg ...> &&>
{
    G(bind_t <calls_, F, Arg ...> && _a) : 
        a(std::move(_a)) 
    {}

    template <typename ... Bs>
    decltype(auto) operator () (Bs && ... bs)
    {
        return a(std::forward <Bs> (bs) ...);
    }
private:
    bind_t <calls_, F, Arg ...> a;
};

template <bool calls, bool calls_, typename F, typename ... Arg>
struct G <calls, bind_t <calls_, F, Arg ...> &>
{
    G(bind_t <calls_, F, Arg ...> & _a) : 
        a(_a) 
    {}

    template <typename ... Bs>
    decltype(auto) operator () (Bs && ... bs)
    {
        return a(std::forward <Bs> (bs) ...);
    }
private:
    bind_t <calls_, F, Arg ...> a;
};

template <bool calls, bool calls_, typename F, typename ... Arg>
struct G <calls, bind_t <calls_, F, Arg ...> const &>
{
    G(bind_t <calls_, F, Arg ...> const & _a) : 
        a(_a) 
    {}

    template <typename ... Bs>
    decltype(auto) operator () (Bs && ... bs)
    {
        return a(std::forward <Bs> (bs) ...);
    }
private:
    bind_t <calls_, F, Arg ...> a;
};

template <bool calls, typename F, typename ... As>
struct bind_t
{
    bind_t(F && _f, As && ... as) : 
        f(std::forward <F> (_f)), 
        gs(std::forward <As> (as) ...)
    {}

    template <typename ... Bs>
    decltype(auto) operator () (Bs && ... bs) 
    {
        return call(typename make_integer_sequence <sizeof ... (As)> :: type (), 
                    typename make_integer_sequence <sizeof ... (Bs)> :: type (), 
                    typename make_IS_from_list <As ...> :: value (),  
                    std::forward <Bs> (bs) ...);
    }
private:
    template <int ... ks, 
              int ... index, 
              int ... PH_index, 
              typename ... Bs>
    decltype(auto) call(integer_sequence <ks ...> as_index, 
                        integer_sequence <index ...> bs_index, 
                        integer_sequence <PH_index ...> placeholder_list,
                        Bs && ... bs)
    {
        return f
        (
            std::get <ks> (gs) 
            (
                 static_cast 
                 <
                    typename get_type 
                    <
                        Bs, 
                        need_forward 
                        <
                            index + 1, 
                            integer_sequence <PH_index ...> 
                        > :: value
                    > :: value
                >
                (bs)...
            )...
        );
    }

    typename get_func_type <F> :: value f;
    std::tuple <G <calls, As> ...> gs;
};

template <typename F, typename ... As>
decltype(auto) bind(F && f, As && ... as)
{
    return bind_t <1, F, typename remove_array <As> :: value ...> 
                  (std::forward <F> (f), (std::forward <As> (as) )...);
}

template <typename F, typename ... As>
decltype(auto) call_once_bind(F && f, As && ... as) 
{
    return bind_t <0, F, typename remove_array <As> :: value ...> 
                  (std::forward <F> (f), (std::forward <As> (as) )...);
}

#endif
