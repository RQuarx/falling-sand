#include "core/simulation.hh"

using kei::core::simulation;


auto
simulation::get_grid() const noexcept -> const grid &
{
    return m_grid;
}


void
simulation::set_size(sdl::size new_size)
{
    m_active_cells.clear();
    m_grid.set_size(new_size);
}


void
simulation::clear()
{
    m_active_cells.clear();
    m_grid.clear();
}


void
simulation::draw(std::span<const sdl::point> cells, int element)
{
    m_active_cells.reserve(cells.size());

    for (const auto &p : cells)
    {
        m_grid[p] = cell { element };
        m_active_cells.emplace_back(p);
    }
}


void
simulation::update(float delta_time)
{

}