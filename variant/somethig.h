
#ifndef SOMETHIG_H
#define SOMETHIG_H

template <size_t I, typename ... Ts>
struct get_type
{};

template <size_t I, typename T, typename ... Ts>
struct get_type <I, T, Ts ...>
{
    typedef typename get_type <I - 1, Ts ...> :: type type;
    typedef typename get_type <I - 1, Ts ...> :: type value;
};

template <typename T, typename ... Ts>
struct get_type <0, T, Ts ...>
{
    typedef T type;
    typedef T value;
};

template <typename T, typename ... tail>
struct get_max_size
{
    static const size_t value = std::max(sizeof (T), get_max_size <tail ...> :: value);
};

template <typename T>
struct get_max_size <T>
{
    static const size_t value = sizeof (T);
};


template <size_t A, size_t B>
struct gcd
{
    static const size_t value = gcd <B, A % B> :: value;
};

template <size_t A>
struct gcd <A, 0>
{
    static const size_t value = A;
};


template <size_t ... tail>
struct get_nok
{};

template <size_t A, size_t B, size_t ... tail>
struct get_nok <A, B, tail ...>
{
    static const size_t value = get_nok <A * B / gcd <A, B> :: value, tail ...> :: value;
};

template <size_t A>
struct get_nok <A>
{
    static const size_t value = A;
};


template <typename ... tail>
struct get_align
{
    static const size_t value = get_nok <sizeof (tail) ...> :: value;
};


template <typename X, typename ... tail>
struct index_of
{};

template <typename X, typename T, typename ... tail>
struct index_of <X, T, tail ...>
{
    static const int value = (std::is_same <X, T> :: value)? 0 : ((index_of <X, tail ...> :: value  == -1)? -1 : index_of <X, tail ...> :: value + 1);
};

template <typename X>
struct index_of <X>
{
    static const int value = -1;
};

template <typename ... Args>
struct wrapper{};

template <typename T0, typename T1, typename ... Args>
struct wrapper<T0, T1, Args...> {
    constexpr wrapper () = default;
    constexpr static void move(void* p, void * to, size_t ind) {
        if (ind == 0) {
            new (to) T0(std::move(* reinterpret_cast <T0*>(p)));
        }
        else {
            wrap.move(p, to, ind - 1);
        }
    }
    constexpr static void destroy(void* p, size_t ind) {
        if (ind == 0) {
            reinterpret_cast<T0*>(p)->~T0();
        }
        else {
            wrap.destroy(p, ind - 1);
        }
    }
    static constexpr wrapper<T1, wrapper<Args...>> wrap = wrapper <T1, wrapper <Args...> > ();
};

template <typename T0>
struct wrapper<T0, wrapper<>> {
    constexpr wrapper () = default;
    constexpr static void destroy(void* p, size_t ind) {
        reinterpret_cast<T0*>(p)->~T0();
    }
    constexpr static void move(void* p, void * to, size_t ind) {
        new (to) T0(std::move(* reinterpret_cast <T0*>(p)));
    }
};

inline constexpr size_t variant_npos = -1;

struct monostate { };
constexpr bool operator<(monostate, monostate) noexcept { return false; }
constexpr bool operator>(monostate, monostate) noexcept { return false; }
constexpr bool operator<=(monostate, monostate) noexcept { return true; }
constexpr bool operator>=(monostate, monostate) noexcept { return true; }
constexpr bool operator==(monostate, monostate) noexcept { return true; }
constexpr bool operator!=(monostate, monostate) noexcept { return false; }

template <template <typename> typename type_checker, typename ... Ts>
struct check
{};

template <template <typename> typename type_checker, typename T0, typename ... Ts>
struct check <type_checker, T0, Ts ...>
{
    static const bool value = type_checker <T0> :: value && check <type_checker, Ts ...> :: value;
};

template <template <typename> typename type_checker>
struct check <type_checker>
{
    static const bool value = 1;
};

template <typename ... Ts>
struct check_copy 
{
    static const bool value = check <std::is_copy_constructible, Ts ...> :: value;
};

template <typename ... Ts>
struct check_move 
{
    static const bool value = check <std::is_move_constructible, Ts ...> :: value;
};

template <typename ... Ts>
struct check_destruct 
{
    static const bool value = check <std::is_trivially_destructible, Ts ...> :: value;
};

template <typename T0, typename T1>
struct is_same
{
    static const bool value = std::is_same <typename std::remove_reference <T0> :: type, typename std::remove_reference <T1> :: type> :: value; 
};
#endif
