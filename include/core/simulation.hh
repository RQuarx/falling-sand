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
        void set_draw_element(int id);
        void clear();

        void draw(std::span<const sdl::point> cells);
        void update(float delta_time);

    private:
        grid m_grid;
        int  m_draw_element;

        std::vector<sdl::point> m_active_cells;
    };
}
