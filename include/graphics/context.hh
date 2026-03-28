#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "error.hh"
#include "sdl/renderer.hh"
#include "sdl/window.hh"
#include "signal/signal.hh"


namespace kei::gfx
{
    class context
    {
        sdl::window                  m_window;
        sdl::renderer                m_render;
        sig::signal<void, context &> m_on_frame_signal;

    public:
        context(const char *title, int width, int height, int flags);


        [[nodiscard]] auto get_window() noexcept -> decltype(m_window) &;
        [[nodiscard]] auto get_render() noexcept -> decltype(m_render) &;
        [[nodiscard]] auto get_fps() const noexcept -> std::uint32_t;
        [[nodiscard]] auto get_dt() const noexcept -> float;

        auto do_frame(std::size_t amount = 1) -> std::optional<error>;


        [[nodiscard]]
        auto signal_on_frame() noexcept
            -> sig::signal_connect<decltype(m_on_frame_signal)>;

    private:
        std::uint64_t m_last_frame_time;
        std::uint32_t m_frame_count;
        std::uint32_t m_fps;
        float         m_dt;
    };
}
