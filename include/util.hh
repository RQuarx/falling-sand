#pragma once
#include <cstddef>
#include <functional>
#include <numeric>


namespace kei::util
{
    namespace _impl
    {
        template <typename T>
        void
        combine_hash(std::size_t &seed, T &&val)
        {
            std::hash<std::remove_reference_t<T>> h {};
            seed ^= h(std::forward<T>(val)) + 0x9e3779b9 + (seed << 6)
                  + (seed >> 2);
        }


        template <typename T, typename... Ts>
        void
        combine_hash(std::size_t &seed, T &&val, Ts &&...rest)
        {
            combine_hash(seed, std::forward<T>(val));
            if constexpr (sizeof...(rest) > 0)
                combine_hash(seed, std::forward<Ts>(rest)...);
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


    template <typename T>
    [[nodiscard]]
    auto
    get_aspect_ratio(T a, T b) -> std::pair<float, float>
        requires std::is_integral_v<T>
    {
        float gcd { static_cast<float>(std::gcd(a, b)) };
        return { a / gcd, b / gcd };
    }
}
