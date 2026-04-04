#pragma once
#include <cmath>
#include <cstddef>
#include <functional>
#include <random>


namespace kei::util
{
    namespace _impl
    {
        template <typename T>
        void
        combine_hash(std::size_t &seed, T &&val)
        {
            std::hash<std::remove_reference_t<T>> h {};
            seed ^= h(std::forward<T>(val)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }


        template <typename T, typename... Ts>
        void
        combine_hash(std::size_t &seed, T &&val, Ts &&...rest)
        {
            combine_hash(seed, std::forward<T>(val));
            if constexpr (sizeof...(rest) > 0) combine_hash(seed, std::forward<Ts>(rest)...);
        }
    }


    template <typename... Ts>
    [[nodiscard]]
    auto
    hash(Ts &&...vals) -> std::size_t
    {
        std::size_t seed { 0 };
        _impl::combine_hash(seed, std::forward<Ts>(vals)...);
        return seed;
    }


    template <typename T, typename U>
    [[nodiscard]]
    auto
    round(U val) -> T
    { return static_cast<T>(std::round(val)); }


    template <typename T>
    [[nodiscard]]
    auto
    rng(T a, T b) -> T
    {
        static std::mt19937 gen { std::random_device {}() };

        if constexpr (std::is_integral_v<T>)
            return std::uniform_int_distribution<T> { a, b }(gen);
        else if constexpr (std::is_floating_point_v<T>)
            return std::uniform_real_distribution<T> { a, b }(gen);
        else
            static_assert(std::is_arithmetic_v<T>, "rng() requires numeric type");
    }
}
