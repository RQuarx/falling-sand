#pragma once
#include <ranges>

#include "sdl/typedefs.hh"


namespace kei::views
{
    namespace _impl
    {
        struct bresenham_view : std::ranges::view_interface<bresenham_view>
        {
            struct iterator
            {
                using iterator_category = std::input_iterator_tag;
                using value_type        = sdl::point;
                using difference_type   = std::ptrdiff_t;

                sdl::point current;
                sdl::point end;
                sdl::point step;
                sdl::point distance;
                int        err;
                bool       done { false };


                iterator() = default;
                iterator(sdl::point a, sdl::point b, bool is_end = false);

                auto operator*() const -> value_type;
                auto operator++() -> iterator &;
                void operator++(int) { ++(*this); }
                auto operator==(const iterator &other) const -> bool;
                auto operator!=(const iterator& other) const -> bool;
            };

            sdl::point a, b;

            bresenham_view() = default;
            bresenham_view(sdl::point a_, sdl::point b_) : a(a_), b(b_) {}

            [[nodiscard]]
            auto
            begin() const -> iterator
            { return iterator { a, b }; }


            [[nodiscard]]
            auto
            end() const -> iterator
            { return iterator { a, b, true }; }
        };


        struct bresenham
        {
            [[nodiscard]]
            constexpr auto
            operator()(sdl::point a, sdl::point b) const
            { return bresenham_view { a, b }; }
        };
    }


    inline constexpr _impl::bresenham bresenham {};
}
