#pragma once
#include <span>

#include "core/grid.hh"


namespace kei::core
{
    class simulation
    {
    public:
        [[nodiscard]] auto get_grid() const noexcept -> const grid &;

        void set_size(sdl::size new_size);
        void clear();

        void draw(std::span<const sdl::point> cells, int element);
        void update(float delta_time);

    private:
        grid m_grid;

        std::vector<sdl::point> m_active_cells;
    };
}
