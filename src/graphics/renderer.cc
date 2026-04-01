#include <numeric>

#include "core/elements.hh"
#include "graphics/renderer.hh"
#include "ui/cursor.hh"

using kei::gfx::renderer;

namespace kei
{
    [[nodiscard]]
    static auto
    get_aspect_ratio(int a, int b) -> std::pair<float, float>
    {
        float gcd { static_cast<float>(std::gcd(a, b)) };
        return { a / gcd, b / gcd };
    }


    [[nodiscard]]
    static auto
    calculate_grid_rect(sdl::size window_size, sdl::size grid_size) -> sdl::frect
    {
        auto       aspect_ratio { get_aspect_ratio(grid_size.w, grid_size.h) };
        sdl::frect final_rect;

        if (window_size.w * aspect_ratio.second <= window_size.h * aspect_ratio.first)
        {
            final_rect.w = window_size.w;
            final_rect.h = window_size.w * aspect_ratio.second / aspect_ratio.first;
        }
        else
        {
            final_rect.h = window_size.h;
            final_rect.w = window_size.h * aspect_ratio.first / aspect_ratio.second;
        }

        sdl::point center {
            .x = window_size.w / 2,
            .y = window_size.h / 2,
        };

        final_rect.x = center.x - (final_rect.w / 2);
        final_rect.y = center.y - (final_rect.h / 2);

        return final_rect;
    }


    [[nodiscard]]
    static auto
    calculate_cell_size(sdl::frect grid_rect, sdl::size grid_size) noexcept -> float
    { return std::min(grid_rect.w / grid_size.w, grid_rect.h / grid_size.h); }


    [[nodiscard]]
    static constexpr auto
    operator!=(const sdl::fsize a, const sdl::size b) noexcept -> bool
    { return a.w != b.w || a.h != b.h; }


    [[nodiscard]]
    static constexpr auto
    get_contrast_color(const sdl::color &c) -> sdl::color
    {
        float brightness { (0.2126F * c.r) + (0.7152F * c.g) + (0.0722F * c.b) };

        if (brightness > 128.0F) return 0xffffff_rgb;
        return 0x000000_rgb;
    }
}


auto
renderer::get_grid_rect() const noexcept -> sdl::frect
{ return m_grid_rect; }


auto
renderer::get_cell_size() const noexcept -> float
{ return m_cell_size; }


auto
renderer::signal_on_size_changed() noexcept
    -> sig::signal_connect<decltype(m_signal_on_size_changed)>
{ return sig::signal_connect { m_signal_on_size_changed }; }


auto
renderer::render_grid(gfx::context &ctx, const core::grid &grid, ui::cursor &cursor)
    -> std::optional<error>
{
    sdl::renderer &renderer { ctx.render() };

    if (auto current_window_size { ctx.window().get_size_in_pixels() })
    {
        if (*current_window_size != m_window_size)
        {
            m_window_size = *current_window_size;
            if (auto e { mf_recalculate(renderer, grid.size()) }) return e;

            m_signal_on_size_changed.emit(grid.size(), {
                                                           .grid_rect = m_grid_rect,
                                                           .cell_size = m_cell_size,
                                                       });
        }
    }
    else
        return current_window_size.error();

    if (auto pixels { m_grid_texture.lock(nullptr) })
    {
        const auto &cursor_border_points { cursor.get_points_to_render(renderer) };

        for (int y { 0 }; y < grid.size().h; y++)
            for (int x { 0 }; x < grid.size().w; x++)
            {
                sdl::color element_color {
                    core::get_element_definition(grid[{ .x = x, .y = y }].element).color
                };

                pixels.value()[y][x] = element_color.to_rgba_uint();
            }

        for (const auto &point : cursor_border_points)
            pixels.value()[point.y][point.x]
                = get_contrast_color(core::get_element_definition(grid[point].element).color)
                      .to_rgba_uint();
    }
    else
        return pixels.error();

    return ctx.render().render_texture(m_grid_texture, nullptr, &m_grid_rect);
}


auto
renderer::mf_recalculate(sdl::renderer &renderer, sdl::size grid_size) -> std::optional<error>
{
    m_grid_rect = calculate_grid_rect(m_window_size, grid_size);
    m_cell_size = calculate_cell_size(m_grid_rect, grid_size);

    if (auto texture_size { m_grid_texture.size() };
        texture_size.has_value() && *texture_size != grid_size)
    {
        if (auto e { renderer.create_texture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                             *texture_size) })
            m_grid_texture.reset(*e);
        else
            return e.error();

        if (auto e { m_grid_texture.set_scale_mode(SDL_SCALEMODE_NEAREST) }) return e;
    }
    else
        return texture_size.error();

    return std::nullopt;
}
