#pragma once
#include <cstdint>
#include <span>
#include <vector>

#include "sdl/typedefs.hh"
#include "simulation/element.hh"


namespace kei::sim
{
    struct cell
    {
        std::int64_t id;
        element_id   element;

        sdl::color color;
        sdl::point pos;

        float       temperature; /* In Kelvin */
        sdl::fpoint velocity;    /* In cell/frame */
    };


    class board
    {
    public:
        [[nodiscard]] auto size() const noexcept -> sdl::size;
        [[nodiscard]] auto operator[](std::size_t x, std::size_t y) -> cell &;
        [[nodiscard]] auto operator[](sdl::point pos) -> cell &;
        [[nodiscard]] auto operator[](std::size_t y) -> std::span<cell>;

        [[nodiscard]] auto operator[](sdl::point pos) const -> const cell &;
        [[nodiscard]]
        auto operator[](std::size_t x, std::size_t y) const -> const cell &;

        void set_size(sdl::size new_size);
        void clear(sdl::point pos);


        template <typename T, typename F>
        void
        for_each_cells(F &&fn)
            requires std::is_integral_v<T>
                  && std::is_invocable_v<F, cell &, T, T>
        {
            for (T y { 0 }; y < static_cast<T>(m_size.h); y++)
                for (T x { 0 }; x < static_cast<T>(m_size.w); x++)
                    fn((*this)[x, y], x, y);
        }


        template <typename T, typename F>
        void
        for_each_cells_reverse(F &&fn)
            requires std::is_integral_v<T>
                  && std::is_invocable_v<F, cell &, T, T>
        {
            for (T y { m_size.h - 1 }; y > 0; y--)
                for (T x { m_size.w - 1 }; x > 0; x--) fn((*this)[x, y], x, y);
        }


        template <typename T, typename F>
        void
        for_each_rows(F &&fn)
            requires std::is_integral_v<T> && std::is_invocable_v<F, cell &, T>
        {
            for (T y { 0 }; y < static_cast<T>(m_size.h); y++)
                fn((*this)[y], y);
        }


    private:
        sdl::size         m_size { .w = 0, .h = 0 };
        std::vector<cell> m_cells;
    };
}
