#pragma once
#include <vector>

#include "cell.hh"


namespace kei::core
{
    class grid
    {
    public:
        using iterator               = std::vector<cell>::iterator;
        using reverse_iterator       = std::vector<cell>::reverse_iterator;
        using const_iterator         = std::vector<cell>::const_iterator;
        using const_reverse_iterator = std::vector<cell>::const_reverse_iterator;


        grid();

        void set_size(sdl::size new_size);
        void clear();

        [[nodiscard]] auto is_in_bounds(sdl::point point) const noexcept -> bool;


        [[nodiscard]] auto size() const noexcept -> sdl::size;

        [[nodiscard]] auto operator[](sdl::point p) -> cell &;
        [[nodiscard]] auto operator[](sdl::point p) const -> const cell &;


        [[nodiscard]] auto begin() noexcept -> iterator;
        [[nodiscard]] auto end() noexcept -> iterator;

        [[nodiscard]] auto begin() const noexcept -> const_iterator;
        [[nodiscard]] auto end() const noexcept -> const_iterator;

        [[nodiscard]] auto cbegin() const noexcept -> const_iterator;
        [[nodiscard]] auto cend() const noexcept -> const_iterator;

        [[nodiscard]] auto rbegin() noexcept -> reverse_iterator;
        [[nodiscard]] auto rend() noexcept -> reverse_iterator;

        [[nodiscard]] auto rbegin() const noexcept -> const_reverse_iterator;
        [[nodiscard]] auto rend() const noexcept -> const_reverse_iterator;

        [[nodiscard]] auto crbegin() const noexcept -> const_reverse_iterator;
        [[nodiscard]] auto crend() const noexcept -> const_reverse_iterator;

    private:
        std::vector<cell> m_cells;
        sdl::size         m_size;
    };
}
