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


        auto get_points_to_draw(sdl::renderer &renderer) -> const std::vector<sdl::point> &;
        auto get_points_to_render(sdl::renderer &renderer) -> const std::vector<sdl::point> &;

    private:
        cursor_shape m_shape { cursor_shape::circle };

        struct mouse_position
        {
            std::optional<sdl::fpoint> previous_position;
            sdl::fpoint                position;
        } m_mouse;

        struct grid_data
        {
            sdl::size        size;
            gfx::grid_layout layout;
        } m_grid;

        sdl::size m_size;

        std::vector<sdl::point> m_draw_points;
        std::vector<sdl::point> m_border_points;


        [[nodiscard]]
        auto mf_mouse_position_to_grid_position(sdl::renderer &renderer, sdl::fpoint position) const
            -> sdl::point;


        void
        mf_for_each_cell_in_shape(sdl::point position, auto &&fn) const
        {
            const sdl::size half { .w = m_size.w / 2, .h = m_size.h / 2 };
            const sdl::size inv_half {
                .w = static_cast<int>(1.F / (half.w > 0 ? half.w : 1)),
                .h = static_cast<int>(1.F / (half.h > 0 ? half.h : 1)),
            };


            for (int y { position.y - half.h }; y <= position.y + half.h; y++)
                for (int x { position.x - half.w }; x <= position.x + half.w; x++)
                {
                    if (x < 0 || y < 0 || x >= m_grid.size.w || y >= m_grid.size.h) continue;

                    sdl::fpoint normalized {
                        .x = static_cast<float>((x - position.x) * inv_half.w),
                        .y = static_cast<float>((y - position.y) * inv_half.h),
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


        auto mf_on_mouse_motion(const sdl::event &event) -> sdl::event_return;
        auto mf_on_mouse_wheel(const sdl::event &event) -> sdl::event_return;
        auto mf_on_key_down(const sdl::event &event) -> sdl::event_return;

        void mf_on_grid_size_changed(sdl::size grid_size, gfx::grid_layout grid_layout);
    };
}
