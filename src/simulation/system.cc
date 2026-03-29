#include <random>

#include "simulation/system.hh"
#include "views/bresenham.hh"

using kei::sim::system;

namespace kei
{
    auto
    randomize_color(const sdl::color &base, int range) -> sdl::color
    {
        static std::mt19937 gen { std::random_device {}() };

        auto rand_channel {
            [range](int c)
            {
                std::uniform_int_distribution<int> dist { -range, range };
                return std::clamp(c + dist(gen), 0, 255);
            }
        };

        sdl::color result;
        result.r = rand_channel(base.r);
        result.g = rand_channel(base.g);
        result.b = rand_channel(base.b);
        result.a = base.a;

        return result;
    }
}


system::system() { m_board.set_size({ .w = 611, .h = 383 }); }


auto
system::board() const noexcept -> const class board &
{ return m_board; }


auto
system::enqueue_draw(const draw_fn &fn) -> system &
{ return fn(m_board.size(), m_draw_queue), *this; }


void
system::update(float dt)
{
    for (auto &cmd : m_draw_queue)
    {
        if (cmd.erase)
        {
            m_board.clear(cmd.point);
            continue;
        }

        cell &cell { m_board[cmd.point.x, cmd.point.y] };

        cell.pos = cmd.point;

        if (cell.element == cmd.id) continue;

        cell.element = cmd.id;
        cell.id      = m_cells.size();
        cell.color
            = randomize_color(element_registry[cmd.id].color,
                              element_registry[cmd.id].color_random_range);

        cell.temperature = 300.0F;
        cell.velocity    = { .x = 0.0F, .y = 0.0F };


        m_cells.emplace_back(std::ref(m_board[cmd.point.x, cmd.point.y]));
    }

    m_draw_queue.clear();


    std::erase_if(
        m_cells,
        [&](std::reference_wrapper<cell> &ref)
        {
            cell              &cell { ref.get() };
            const element_def &element { element_registry[cell.element] };

            sdl::point start { cell.pos };

            cell.velocity.y += G * dt;

            sdl::point target {
                .x
                = start.x + static_cast<int>(std::round(cell.velocity.x * dt)),
                .y
                = start.y + static_cast<int>(std::round(cell.velocity.y * dt))
            };

            target.x = std::clamp(target.x, 0, m_board.size().w - 1);
            target.y = std::clamp(target.y, 0, m_board.size().h - 1);

            sdl::point last_valid { start };

            for (auto p : views::bresenham(start, target) | std::views::drop(1))
            {
                if (m_board[p].element != element_registry->air) break;
                last_valid = p;
            }

            if (last_valid == start)
            {
                cell.velocity = { .x = 0.F, .y = 0.F };
                return false;
            }

            std::swap(m_board[start], m_board[last_valid]);
            ref           = std::ref(m_board[last_valid]);
            ref.get().pos = last_valid;

            if (last_valid != target)
            {
                cell.velocity.x = 0.F;
                cell.velocity.y = 0.F;
            }

            return false;
        });
}


auto
system::get_present_cells() const
    -> std::span<const std::reference_wrapper<cell>>
{ return m_cells; }
