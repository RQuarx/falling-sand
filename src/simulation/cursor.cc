#include <array>
#include <cstdlib>

#include "simulation/cursor.hh"
#include "simulation/system.hh"

using kei::sim::cursor;


void
cursor::draw(sdl::size                  board_size,
             std::vector<draw_command> &cells_to_draw,
             bool                       erase) const noexcept
{
    sdl::point min_pos { .x = static_cast<int>(pos.x - (size.w / 2)),
                         .y = static_cast<int>(pos.y - (size.h / 2)) };
    sdl::point max_pos { .x = static_cast<int>(pos.x + (size.w / 2)),
                         .y = static_cast<int>(pos.y + (size.h / 2)) };


    for (int y { min_pos.y }; y <= max_pos.y; y++)
        for (int x { min_pos.x }; x <= max_pos.x; x++)
        {
            if (x < 0 || y < 0 || x >= board_size.w || y >= board_size.h)
                continue;

            sdl::fpoint normalized { .x = (x - pos.x) / (size.w / 2.0F),
                                     .y = (y - pos.y) / (size.h / 2.0F) };

            if (mf_is_inside(normalized))
                cells_to_draw.emplace_back(sdl::point { .x = x, .y = y },
                                           draw_id, erase);
        }
}


void
cursor::get_border_indices(sdl::size                board_size,
                           std::vector<sdl::point> &indices) const noexcept
{
    sdl::point min_pos { .x = static_cast<int>(pos.x - (size.w / 2)),
                         .y = static_cast<int>(pos.y - (size.h / 2)) };
    sdl::point max_pos { .x = static_cast<int>(pos.x + (size.w / 2)),
                         .y = static_cast<int>(pos.y + (size.h / 2)) };

    constexpr std::array dx { 1, -1, 0, 0 };
    constexpr std::array dy { 0, 0, 1, -1 };

    for (int y { min_pos.y }; y <= max_pos.y; y++)
        for (int x { min_pos.x }; x <= max_pos.x; x++)
        {
            if (x < 0 || y < 0 || x >= board_size.w || y >= board_size.h)
                continue;

            sdl::fpoint normalized { .x = (x - pos.x) / (size.w / 2.0F),
                                     .y = (y - pos.y) / (size.h / 2.0F) };

            if (!mf_is_inside(normalized)) continue;

            bool is_border { false };

            for (int i { 0 }; i < 4; i++)
            {
                sdl::point  neighbor { .x = x + dx[i], .y = y + dy[i] };
                sdl::fpoint normalized {
                    .x = (neighbor.x - pos.x) / (size.w / 2.0F),
                    .y = (neighbor.y - pos.y) / (size.h / 2.0F)
                };

                if (!mf_is_inside(normalized))
                {
                    is_border = true;
                    break;
                }
            }

            if (is_border) indices.emplace_back(x, y);
        }
}


auto
cursor::mf_is_inside(sdl::fpoint normalized_pos) const noexcept -> bool
{
    switch (shape)
    {
    case shape::square:
        return std::abs(normalized_pos.x) <= 1.0F
            && std::abs(normalized_pos.y) <= 1.0F;

    case shape::circle:
        return ((normalized_pos.x * normalized_pos.x)
                + (normalized_pos.y * normalized_pos.y))
            <= 1.0F;

    case shape::triangle:
        normalized_pos.y = -normalized_pos.y;

        return normalized_pos.y >= -1.0F && normalized_pos.y <= 1.0F
            && std::abs(normalized_pos.x) <= (1.0F - normalized_pos.y) / 2.0F;

    default: return false;
    }
}
