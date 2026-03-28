#include <SDL3/SDL.h>

#include "graphics/context.hh"
#include "sdl/error.hh"

#define DOMAIN "gfx::context"

using kei::gfx::context;


context::context(const char *title, int width, int height, int flags)
    : m_last_frame_time { SDL_GetTicks() }, m_frame_count { 0 }
{
    SDL_Window   *window { nullptr };
    SDL_Renderer *render { nullptr };

    if (!SDL_CreateWindowAndRenderer(title, width, height, flags, &window,
                                     &render))
        throw sdl::error_builder[DOMAIN]();

    m_window.reset(window);
    m_render.reset(render);

    if (!SDL_SetRenderVSync(m_render.get(), 1))
        throw sdl::error_builder[DOMAIN]();
}


auto
context::get_window() noexcept -> decltype(m_window) &
{
    return m_window;
}


auto
context::get_render() noexcept -> decltype(m_render) &
{
    return m_render;
}


auto
context::get_fps() const noexcept -> std::uint32_t
{
    return m_fps;
}


auto
context::get_dt() const noexcept -> float
{
    return m_dt;
}


auto
context::signal_on_frame() noexcept
    -> sig::signal_connect<decltype(m_on_frame_signal)>
{
    return sig::signal_connect { m_on_frame_signal };
}


auto
context::do_frame(std::size_t amount) -> std::optional<error>
{
    for (; amount > 0; amount--)
    {
        auto current_time { SDL_GetTicks() };
        m_dt = (current_time - m_last_frame_time) / 1000.0F; /* seconds */
        m_last_frame_time = current_time;

        if (auto e { m_render.set_draw_color(0x000000_rgb) }) return e;
        if (auto e { m_render.clear() }) return e;

        try
        {
            m_on_frame_signal.emit(*this);
        }
        catch (const error &err)
        {
            return err;
        }

        if (auto e { m_render.present() }) return e;

        m_frame_count++;

        m_frame_count++;
        if (m_frame_count % 60 == 0)
        {
            m_fps = static_cast<std::uint32_t>(1.0F / m_dt);
        }
    }

    return std::nullopt;
}
