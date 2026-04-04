#include <cstddef>

#include "signal/method.hh"
#include "ui/cursor.hh"
#include "views/bresenham.hh"

using kei::ui::cursor;


void
cursor::connect_signals(sdl::event_handler &event_handler, gfx::renderer &renderer)
{
    event_handler[SDL_EVENT_MOUSE_WHEEL] | sig::method(*this, &cursor::mf_on_mouse_wheel);
    event_handler[SDL_EVENT_KEY_DOWN] | sig::method(*this, &cursor::mf_on_key_down);

    renderer.signal_on_size_changed() | sig::method(*this, &cursor::mf_on_grid_layout_changed);
}


auto
cursor::get_points_to_draw(sdl::fpoint mouse_pos1, sdl::fpoint mouse_pos2, sdl::renderer &renderer)
    -> const std::vector<sdl::point> &
{
    m_draw_points.clear();
    mf_update_visited();

    sdl::point previous_position { mouse_position_to_grid_position(renderer, mouse_pos1) };
    sdl::point current_position { mouse_position_to_grid_position(renderer, mouse_pos2) };

    for (auto pos : views::bresenham(m_previous_position, current_position))
        mf_draw(pos, m_draw_points);
    for (auto pos : views::bresenham(previous_position, current_position))
        mf_draw(pos, m_draw_points);

    m_previous_position = current_position;
    return m_draw_points;
}


auto
cursor::get_points_to_draw(sdl::fpoint mouse_pos, sdl::renderer &renderer)
    -> const std::vector<sdl::point> &
{
    m_draw_points.clear();
    mf_update_visited();

    m_previous_position = mouse_position_to_grid_position(renderer, mouse_pos);

    mf_draw(m_previous_position, m_draw_points);
    return m_draw_points;
}


auto
cursor::get_points_to_render(sdl::fpoint mouse_pos, sdl::renderer &renderer)
    -> const std::vector<sdl::point> &
{
    m_border_points.clear();
    sdl::point position { mouse_position_to_grid_position(renderer, mouse_pos) };
    sdl::fsize half_size { (m_size.w - 0.5F) / 2.F, (m_size.h - 0.5F) / 2.F };

    constexpr std::array dx { 1, -1, 0, 0 };
    constexpr std::array dy { 0, 0, 1, -1 };

    mf_for_each_cell_in_shape(
        position,
        [&](int x, int y, auto)
        {
            bool is_border { false };

            for (int i { 0 }; i < 4; i++)
            {
                sdl::point  neighbor { .x = x + dx[i], .y = y + dy[i] };
                sdl::fpoint normalized { .x = (neighbor.x - position.x) / half_size.w,
                                         .y = (neighbor.y - position.y) / half_size.h };

                if (!mf_is_inside_shape(normalized))
                {
                    is_border = true;
                    break;
                }
            }

            if (is_border) m_border_points.emplace_back(x, y);
        });

    return m_border_points;
}


auto
cursor::mouse_position_to_grid_position(sdl::renderer &renderer, sdl::fpoint position) const
    -> sdl::point
{
    sdl::fpoint mouse_pos { renderer.render_position_from_window(position) };
    sdl::fpoint relative_pos {
        .x
        = std::clamp(mouse_pos.x - m_grid.layout.grid_rect.x, 0.F, m_grid.layout.grid_rect.w - 1),
        .y
        = std::clamp(mouse_pos.y - m_grid.layout.grid_rect.y, 0.F, m_grid.layout.grid_rect.h - 1),
    };

    return {
        .x = static_cast<int>(relative_pos.x / m_grid.layout.cell_size),
        .y = static_cast<int>(relative_pos.y / m_grid.layout.cell_size),
    };
}


auto
cursor::mf_is_inside_shape(sdl::fpoint normalized_pos) const noexcept -> bool
{
    switch (m_shape)
    {
    case cursor_shape::square:
        return std::abs(normalized_pos.x) <= 1.0F && std::abs(normalized_pos.y) <= 1.0F;

    case cursor_shape::circle:
        return ((normalized_pos.x * normalized_pos.x) + (normalized_pos.y * normalized_pos.y))
            <= 1.0F;

    case cursor_shape::triangle:
        normalized_pos.y = -normalized_pos.y;

        return normalized_pos.y >= -1.0F && normalized_pos.y <= 1.0F
            && std::abs(normalized_pos.x) <= (1.0F - normalized_pos.y) / 2.0F;

    default: return false;
    }
}


void
cursor::mf_update_visited()
{
    auto grid_size { static_cast<std::size_t>(m_grid.size.w) * m_grid.size.h };
    if (m_visited.size() != grid_size) m_visited.resize(grid_size);
    std::ranges::fill(m_visited, false);
}


void
cursor::mf_draw(sdl::point position, std::vector<sdl::point> &points) noexcept
{
    mf_for_each_cell_in_shape(position,
                              [&](int x, int y, auto)
                              {
                                  auto idx { static_cast<std::size_t>((y * m_grid.size.w) + x) };
                                  if (m_visited[idx]) return;
                                  m_visited[idx] = true;

                                  points.emplace_back(x, y);
                              });
}


auto
cursor::mf_on_mouse_wheel(const sdl::event &event) -> sdl::event_return
{
    int delta { static_cast<int>(std::round(event.wheel.y)) };

    if ((SDL_GetModState() & SDL_KMOD_SHIFT) != 0U)
        m_size.w = std::max(1, m_size.w + delta);
    else if ((SDL_GetModState() & SDL_KMOD_CTRL) != 0U)
        m_size.h = std::max(1, m_size.h + delta);
    else
    {
        m_size.w = std::max(1, m_size.w + delta);
        m_size.h = std::max(1, m_size.h + delta);
    }

    return sdl::event_return::success;
}


auto
cursor::mf_on_key_down(const sdl::event &event) -> sdl::event_return
{
    switch (event.key.scancode)
    {
    case SDL_SCANCODE_TAB:
        switch (m_shape)
        {
        case cursor_shape::circle:   m_shape = cursor_shape::square; break;
        case cursor_shape::square:   m_shape = cursor_shape::triangle; break;
        case cursor_shape::triangle: m_shape = cursor_shape::circle; break;
        }
        return sdl::event_return::success;
    default: return sdl::event_return::success;
    }
}


void
cursor::mf_on_grid_layout_changed(sdl::size grid_size, gfx::grid_layout grid_layout)
{
    m_grid.size   = grid_size;
    m_grid.layout = grid_layout;
}
