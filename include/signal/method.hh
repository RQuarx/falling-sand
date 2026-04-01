#pragma once


namespace kei::sig
{
    template <typename T_Instance, typename T_Ret, typename... T_Params>
    [[nodiscard]]
    constexpr auto
    method(T_Instance &instance, T_Ret (T_Instance::*mem_fn)(T_Params...))
    {
        return [&instance, mem_fn](auto &&...params) -> T_Ret
        { return (instance.*mem_fn)(decltype(params)(params)...); };
    }


    template <typename T_Instance, typename T_Ret, typename... T_Params>
    [[nodiscard]]
    constexpr auto
    method(const T_Instance &instance, T_Ret (T_Instance::*mem_fn)(T_Params...) const)
    {
        return [&instance, mem_fn](auto &&...params) -> T_Ret
        { return (instance.*mem_fn)(decltype(params)(params)...); };
    }
}
