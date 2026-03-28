#pragma once
#include <array>
#include <cstdint>
#include <utility>


namespace kei::gfx
{
    enum class edge : std::uint8_t
    {
        top    = 0,
        end    = 1,
        bottom = 2,
        start  = 3,
        left   = start,
        right  = end,
    };


    template <typename T> class edge_data
    {
    public:
        edge_data(T top, T end, T bottom, T start)
            : m_data { std::move(top), std::move(end), std::move(bottom),
                       std::move(start) }
        {
        }

        edge_data() = default;


        edge_data(const edge_data &other) = default;
        edge_data(edge_data &&other) noexcept
            : m_data { std::move(other.m_data) }
        {
        }


        auto
        operator=(const edge_data &other) -> edge_data &
        {
            if (this != &other) m_data = other.m_data;
            return *this;
        }


        auto
        operator=(edge_data &&other) noexcept -> edge_data &
        {
            if (this != &other) m_data = std::move(other.m_data);
            return *this;
        }


        [[nodiscard]]
        auto
        operator[](edge edge) const noexcept -> const T &
        {
            return m_data[std::to_underlying(edge)];
        }


        [[nodiscard]]
        auto
        operator[](edge edge) noexcept -> T &
        {
            return m_data[std::to_underlying(edge)];
        }

    private:
        std::array<T, 4> m_data;
    };
}
