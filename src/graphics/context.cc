#include <SDL3/SDL.h>

#include "graphics/context.hh"
#include "sdl/error.hh"

#define DOMAIN "gfx::context"

using kei::gfx::context;


context::context(const char *title, int width, int height, int flags)
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
context::window() noexcept -> sdl::window &
{ return m_window; }


auto
context::render() noexcept -> sdl::renderer &
{ return m_render; }


auto
context::signal_on_frame() noexcept
    -> sig::signal_connect<decltype(m_on_frame_signal)>
{ return sig::signal_connect { m_on_frame_signal }; }


auto
context::do_frame(sdl::color bg) -> std::optional<error>
{
    if (auto e { m_render.set_draw_color(bg) }) return e;
    if (auto e { m_render.clear() }) return e;

    try
    {
        m_on_frame_signal.emit(*this);
    }
    catch (const error &err)
    {
        return err;
    }

    return m_render.present();
}
