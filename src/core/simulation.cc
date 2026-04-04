#include "core/elements.hh"
#include "core/simulation.hh"
#include "util.hh"
#include "views/bresenham.hh"

using kei::core::simulation;

namespace kei
{
    static auto
    randomize_color(const sdl::color &base, int range) -> sdl::color
    {
        if (range == 0) return base;

        auto rand_channel { [range](int c)
                            {
                                return std::clamp(c + util::rng(-range, range), 0, 255);
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
        mf_set_point_to(p, m_draw_element);
        m_active_cells.emplace_back(p);
    }
}


void
simulation::update(float delta_time)
{
    std::erase_if(m_active_cells,
                  [&](sdl::point &point)
                  {
                      cell       &cell { m_grid[point] };
                      const auto &element { get_element_definition(cell.element) };

                      cell.velocity.y += GRAVITY * delta_time;

                      sdl::point next_position { point.x,
                                                 point.y + util::round<int>(cell.velocity.y) };

                      if (!m_grid.is_in_bounds(next_position))
                      {
                          mf_set_point_to(point, elements::air.id);
                          return true;
                      }

                      sdl::point last_valid_pos { point };
                      sdl::point blocking_pos;
                      for (auto p : views::bresenham(point, next_position) | std::views::drop(1))
                      {
                          if (m_grid[p].element != elements::air.id)
                          {
                              blocking_pos = p;
                              break;
                          }

                          last_valid_pos = p;
                      }

                      if (last_valid_pos != next_position) { cell.velocity.y = 0; }

                      std::swap(m_grid[point], m_grid[last_valid_pos]);
                      point = last_valid_pos;

                      return false;
                  });
}


void
simulation::mf_set_point_to(sdl::point point, int element)
{
    cell &c { m_grid[point] };

    if (c.element == element) return;

    c = cell { element };

    auto element_definition { core::get_element_definition(element) };
    c.color = randomize_color(element_definition.color, element_definition.random_color_range);
}
