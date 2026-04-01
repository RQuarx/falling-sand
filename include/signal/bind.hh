#pragma once
#include <tuple>
#include <type_traits>
#include <utility>


namespace kei::sig
{
    namespace bind_to
    {
        struct position_1
        {
        };

        struct position_2
        {
        };

        struct position_3
        {
        };

        struct position_4
        {
        };

        struct position_5
        {
        };

        struct position_6
        {
        };

        struct position_7
        {
        };

        struct position_8
        {
        };

        struct position_9
        {
        };
    }


    namespace _impl
    {
        template <typename T> struct is_placeholder : std::false_type
        {
        };

        template <> struct is_placeholder<bind_to::position_1> : std::true_type
        {
        };

        template <> struct is_placeholder<bind_to::position_2> : std::true_type
        {
        };

        template <> struct is_placeholder<bind_to::position_3> : std::true_type
        {
        };

        template <> struct is_placeholder<bind_to::position_4> : std::true_type
        {
        };

        template <> struct is_placeholder<bind_to::position_5> : std::true_type
        {
        };

        template <> struct is_placeholder<bind_to::position_6> : std::true_type
        {
        };

        template <> struct is_placeholder<bind_to::position_7> : std::true_type
        {
        };

        template <> struct is_placeholder<bind_to::position_8> : std::true_type
        {
        };

        template <> struct is_placeholder<bind_to::position_9> : std::true_type
        {
        };

        template <typename T> inline constexpr bool is_placeholder_v = is_placeholder<T>::value;


        template <typename Param, typename CallArgs>
        decltype(auto)
        resolve_arg(Param &&param, CallArgs && /* unused */)
        { return std::forward<Param>(param); }


        template <typename CallArgs>
        decltype(auto)
        resolve_arg(bind_to::position_1 /* unused */, CallArgs &&call_args)
        { return std::get<0>(std::forward<CallArgs>(call_args)); }

        template <typename CallArgs>
        decltype(auto)
        resolve_arg(bind_to::position_2 /* unused */, CallArgs &&call_args)
        { return std::get<1>(std::forward<CallArgs>(call_args)); }

        template <typename CallArgs>
        decltype(auto)
        resolve_arg(bind_to::position_3 /* unused */, CallArgs &&call_args)
        { return std::get<2>(std::forward<CallArgs>(call_args)); }

        template <typename CallArgs>
        decltype(auto)
        resolve_arg(bind_to::position_4 /* unused */, CallArgs &&call_args)
        { return std::get<3>(std::forward<CallArgs>(call_args)); }

        template <typename CallArgs>
        decltype(auto)
        resolve_arg(bind_to::position_5 /* unused */, CallArgs &&call_args)
        { return std::get<4>(std::forward<CallArgs>(call_args)); }

        template <typename CallArgs>
        decltype(auto)
        resolve_arg(bind_to::position_6 /* unused */, CallArgs &&call_args)
        { return std::get<5>(std::forward<CallArgs>(call_args)); }

        template <typename CallArgs>
        decltype(auto)
        resolve_arg(bind_to::position_7 /* unused */, CallArgs &&call_args)
        { return std::get<6>(std::forward<CallArgs>(call_args)); }

        template <typename CallArgs>
        decltype(auto)
        resolve_arg(bind_to::position_8 /* unused */, CallArgs &&call_args)
        { return std::get<7>(std::forward<CallArgs>(call_args)); }

        template <typename CallArgs>
        decltype(auto)
        resolve_arg(bind_to::position_9 /* unused */, CallArgs &&call_args)
        { return std::get<8>(std::forward<CallArgs>(call_args)); }
    }


    template <typename Func, typename... Args>
    auto
    bind(Func &&f, Args &&...bound_args)
    {
        auto bound_tuple { std::make_tuple(std::forward<Args>(bound_args)...) };

        return [f { std::forward<Func>(f) },
                bound_tuple { std::move(bound_tuple) }](auto &&...call_args) mutable
        {
            auto call_tuple { std::forward_as_tuple(call_args...) };

            return std::apply(
                [&](auto &&...args)
                {
                    return f(_impl::resolve_arg(std::forward<decltype(args)>(args), call_tuple)...);
                },
                bound_tuple);
        };
    }
}
