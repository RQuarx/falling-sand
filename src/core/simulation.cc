#include <random>

#include "core/elements.hh"
#include "core/simulation.hh"

using kei::core::simulation;

namespace kei
{
    static auto
    randomize_color(const sdl::color &base, int range) -> sdl::color
    {
        static std::mt19937 gen { std::random_device {}() };

        auto rand_channel { [range](int c)
                            {
                                std::uniform_int_distribution<int> dist { -range, range };
                                return std::clamp(c + dist(gen), 0, 255);
                            } };

        sdl::color result;
        result.r = rand_channel(base.r);
        result.g = rand_channel(base.g);
        result.b = rand_channel(base.b);
        result.a = base.a;

        return result;
    }
}


auto
simulation::get_grid() const noexcept -> const grid &
{ return m_grid; }


void
simulation::set_draw_element(int id)
{ m_draw_element = id; }


auto
simulation::get_draw_element() const noexcept -> int
{ return m_draw_element; }


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
simulation::draw(std::span<const sdl::point> cells)
{
    m_active_cells.reserve(cells.size());

    for (const auto &p : cells)
    {
        cell &c { m_grid[p] };

        if (c.element == m_draw_element) continue;

        c = cell { m_draw_element };

        auto element { core::get_element_definition(m_draw_element) };
        c.color = randomize_color(element.color, element.random_color_range);
        m_active_cells.emplace_back(p);
    }
}


void
simulation::update(float delta_time)
{
}
