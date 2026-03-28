#pragma once
#include "error.hh"
#include "graphics/context.hh"
#include "sdl/texture.hh"
#include "sdl/typedefs.hh"
#include "simulation/cursor.hh"


namespace kei::sim { struct cell; }

namespace kei::gfx
{
    class renderer
    {
    public:
        [[nodiscard]] auto get_board_rect() const noexcept -> sdl::frect;
        [[nodiscard]] auto get_cell_size() const noexcept -> float;

        auto render(context  &ctx,
                    sdl::size board_size,
                    std::span<const std::reference_wrapper<sim::cell>> cells,
                    const sim::cursor &cursor) -> std::optional<error>;

    private:
        sdl::texture m_board_texture { nullptr };

        sdl::frect m_board_rect;
        float      m_cell_size;

        sdl::point              m_last_cursor_pos;
        sdl::size               m_last_cursor_size;
        std::vector<sdl::point> m_cursor_borders;

        auto mf_recalculate(context &ctx, sdl::size board_size)
            -> std::optional<error>;
    };
}
