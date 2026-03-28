#include "graphics/renderer.hh"
#include "logger.hh"
#include "simulation/board.hh"
#include "util.hh"

using kei::gfx::renderer;

namespace kei
{
    static sdl::size max_size;


    static auto
    get_max_board_rect(sdl::size board_size) noexcept -> sdl::frect
    {
        auto aspect_ratio { util::get_aspect_ratio(board_size.w,
                                                   board_size.h) };

        sdl::frect final_rect;

        if (max_size.w * aspect_ratio.second <= max_size.h * aspect_ratio.first)
        {
            final_rect.w = max_size.w;
            final_rect.h
                = max_size.w * aspect_ratio.second / aspect_ratio.first;
        }
        else
        {
            final_rect.h = max_size.h;
            final_rect.w
                = max_size.h * aspect_ratio.first / aspect_ratio.second;
        }

        sdl::point center { max_size.w / 2, max_size.h / 2 };

        final_rect.x = center.x - (final_rect.w / 2);
        final_rect.y = center.y - (final_rect.h / 2);

        return final_rect;
    }


    static auto
    get_cell_size(sdl::frect max_board_rect, sdl::size board_size) noexcept
        -> float
    {
        return std::min(max_board_rect.w / board_size.w,
                        max_board_rect.h / board_size.h);
    }
}


auto
renderer::mf_recalculate(context &ctx, sdl::size board_size)
    -> std::optional<class error>
{
    m_board_rect = get_max_board_rect(board_size);
    m_cell_size  = kei::get_cell_size(m_board_rect, board_size);

    if (auto size { m_board_texture.size() }; !size)
        return size.error();
    else if (size->w != board_size.w || size->h != board_size.h) /* NOLINT */
    {
        m_board_texture.reset(SDL_CreateTexture(
            ctx.get_render().get(), SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING, board_size.w, board_size.h));

        if (auto e { m_board_texture.set_scale_mode(SDL_SCALEMODE_NEAREST) })
            return e;
    }

    return std::nullopt;
}


auto
renderer::get_board_rect() const noexcept -> sdl::frect
{
    return m_board_rect;
}


auto
renderer::get_cell_size() const noexcept -> float
{
    return m_cell_size;
}


auto
renderer::render(context                                           &ctx,
                 sdl::size                                          board_size,
                 std::span<const std::reference_wrapper<sim::cell>> cells,
                 const sim::cursor &cursor) -> std::optional<class error>
{
    sdl::size window_size;
    if (auto e { ctx.get_window().get_size_in_pixels() })
        window_size = *e;
    else
        return e.error();

    if (max_size != window_size)
    {
        max_size = window_size;
        if (auto e { mf_recalculate(ctx, board_size) }) return e;
    }

    if (m_last_cursor_pos != cursor.pos || m_last_cursor_size != cursor.size)
    {
        m_cursor_borders.clear();
        cursor.get_border_indices(board_size, m_cursor_borders);
        m_last_cursor_pos  = cursor.pos;
        m_last_cursor_size = cursor.size;
    }

    {
        auto pixels { m_board_texture.lock(nullptr) };

        if (!pixels) return pixels.error();


        for (int y { 0 }; y < board_size.h; y++)
            for (int x { 0 }; x < board_size.w; x++)
                pixels.value()[y][x] = 0;

        for (const auto &cell : cells)
            pixels.value()[cell.get().pos.y][cell.get().pos.x]
                = cell.get().color.to_rgba_uint();

        for (const auto &index : m_cursor_borders)
            pixels.value()[index.y][index.x] = 0xffffffff;
    }

    return ctx.get_render().render_texture(m_board_texture, nullptr,
                                           &m_board_rect);
}
