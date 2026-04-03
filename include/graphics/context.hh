#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "sdl/renderer.hh"
#include "sdl/window.hh"
#include "signal/signal.hh"


namespace kei::gfx
{
    class context
    {
    public:
        context(const char *title, int width, int height, int flags);


        [[nodiscard]] auto window() noexcept -> sdl::window &;
        [[nodiscard]] auto renderer() noexcept -> sdl::renderer &;

        void do_frame(sdl::color bg);


        [[nodiscard]]
        auto signal_on_frame() noexcept
            -> sig::signal_connect<sig::signal<void, context &>>;

    private:
        sdl::window                  m_window;
        sdl::renderer                m_renderer;
        sig::signal<void, context &> m_on_frame_signal;
    };
}
