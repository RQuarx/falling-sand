#include <SDL3/SDL.h>

#include "graphics/context.hh"
#include "sdl/error.hh"

#define DOMAIN "gfx::context"

using kei::gfx::context;


context::context(const char *title, int width, int height, int flags)
{
    SDL_Window   *window { nullptr };
    SDL_Renderer *render { nullptr };

    if (!SDL_CreateWindowAndRenderer(title, width, height, flags, &window, &render))
        throw sdl::error_builder[DOMAIN]();

    m_window.reset(window);
    m_renderer.reset(render);

    if (!SDL_SetRenderVSync(m_renderer.get(), 1)) throw sdl::error_builder[DOMAIN]();
}


auto
context::window() noexcept -> sdl::window &
{ return m_window; }


auto
context::renderer() noexcept -> sdl::renderer &
{ return m_renderer; }


auto
context::signal_on_frame() noexcept -> sig::signal_connect<decltype(m_on_frame_signal)>
{ return sig::signal_connect { m_on_frame_signal }; }


void
context::do_frame(sdl::color bg)
{
    m_renderer.set_draw_color(bg);
    m_renderer.clear();

    m_on_frame_signal.emit(*this);

    m_renderer.present();
}
