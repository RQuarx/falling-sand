#pragma once
#include "core/grid.hh"
#include "graphics/context.hh"
#include "sdl/texture.hh"

namespace kei::ui { class cursor; }


namespace kei::gfx
{
    struct grid_layout
    {
        sdl::frect grid_rect { .x = 1, .y = 1, .w = 1, .h = 1 };
        float      cell_size { 1 };
    };


    class renderer
    {
        sig::signal<void, sdl::size, grid_layout> m_signal_on_size_changed;

    public:
        [[nodiscard]] auto get_layout() const noexcept -> grid_layout;

        [[nodiscard]]
        auto signal_on_size_changed() noexcept
            -> sig::signal_connect<decltype(m_signal_on_size_changed)>;

        void render_grid_to_texture(gfx::context &ctx, const core::grid &grid);
        void render_cursor_to_texture(gfx::context     &ctx,
                                      const core::grid &grid,
                                      ui::cursor       &cursor,
                                      sdl::fpoint       mouse_pos);

        void render_texture(gfx::context &ctx);

    private:
        sdl::size    m_window_size;
        sdl::texture m_grid_texture;

        grid_layout m_layout;


        void mf_recalculate(sdl::renderer &renderer, sdl::size grid_size);
    };
}
