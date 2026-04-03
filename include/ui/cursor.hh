#pragma once
#include <cstdint>
#include <optional>

#include "graphics/renderer.hh"
#include "sdl/event.hh"
#include "sdl/typedefs.hh"

namespace kei::core { class grid; }
namespace kei::sdl { struct renderer; }


namespace kei::ui
{
    class cursor
    {
        enum class cursor_shape : std::uint8_t
        {
            circle,
            square,
            triangle,
        };

    public:
        void connect_signals(sdl::event_handler &event_handler, gfx::renderer &renderer);


        [[nodiscard]]
        auto get_points_to_draw(sdl::fpoint    mouse_pos1,
                                sdl::fpoint    mouse_pos2,
                                sdl::renderer &renderer) -> const std::vector<sdl::point> &;

        [[nodiscard]]
        auto get_points_to_draw(sdl::fpoint mouse_pos, sdl::renderer &renderer)
            -> const std::vector<sdl::point> &;

        [[nodiscard]]
        auto get_points_to_render(sdl::fpoint mouse_pos, sdl::renderer &renderer)
            -> const std::vector<sdl::point> &;

        [[nodiscard]]
        auto mouse_position_to_grid_position(sdl::renderer &renderer, sdl::fpoint position) const
            -> sdl::point;

    private:
        cursor_shape m_shape { cursor_shape::circle };

        struct grid_data
        {
            sdl::size        size;
            gfx::grid_layout layout;
        } m_grid;

        sdl::size m_size { .w = 10, .h = 10 };

        std::vector<sdl::point> m_draw_points;
        std::vector<sdl::point> m_border_points;


        void
        mf_for_each_cell_in_shape(sdl::point position, auto &&fn) const
        {
            const sdl::size half { .w = static_cast<int>(m_size.w - 0.5F) / 2,
                                   .h = static_cast<int>(m_size.h - 0.5F) / 2 };

            for (int y { position.y - half.h }; y <= position.y + half.h; y++)
                for (int x { position.x - half.w }; x <= position.x + half.w; x++)
                {
                    if (x < 0 || y < 0 || x >= m_grid.size.w || y >= m_grid.size.h) continue;

                    sdl::fpoint normalized {
                        .x = (x - position.x) / (m_size.w / 2.F),
                        .y = (y - position.y) / (m_size.h / 2.F),
                    };

                    if (!mf_is_inside_shape(normalized)) continue;

                    fn(x, y, normalized);
                }
        }


        [[nodiscard]]
        auto mf_is_inside_shape(sdl::fpoint normalized_position) const noexcept -> bool;
        void mf_draw(sdl::point               position,
                     std::vector<sdl::point> &points,
                     std::vector<bool>       *visited) const noexcept;


        auto mf_on_mouse_wheel(const sdl::event &event) -> sdl::event_return;
        auto mf_on_key_down(const sdl::event &event) -> sdl::event_return;

        void mf_on_grid_layout_changed(sdl::size grid_size, gfx::grid_layout grid_layout);
    };
}
