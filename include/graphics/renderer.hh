#pragma once
#include "core/grid.hh"
#include "graphics/context.hh"
#include "sdl/texture.hh"

namespace kei::ui { class cursor; }


namespace kei::gfx
{
    struct grid_layout
    {
        sdl::frect grid_rect;
        float      cell_size;
    };


    class renderer
    {
        sig::signal<void, sdl::size, grid_layout> m_signal_on_size_changed;

    public:
        [[nodiscard]] auto get_grid_rect() const noexcept -> sdl::frect;
        [[nodiscard]] auto get_cell_size() const noexcept -> float;

        [[nodiscard]]
        auto signal_on_size_changed() noexcept
            -> sig::signal_connect<decltype(m_signal_on_size_changed)>;


        void render_grid(gfx::context &ctx, const core::grid &grid, ui::cursor &cursor);

    private:
        sdl::size    m_window_size;
        sdl::texture m_grid_texture;

        sdl::frect m_grid_rect;
        float      m_cell_size;


        void mf_recalculate(sdl::renderer &renderer, sdl::size grid_size);
    };
}
