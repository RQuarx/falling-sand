#pragma once
#include "graphics/context.hh"
#include "graphics/renderer.hh"
#include "sdl/event.hh"
#include "simulation/cursor.hh"
#include "simulation/system.hh"


namespace kei
{
    class game
    {
    public:
        game();
        ~game();


        auto run() noexcept -> int;

    private:
        bool m_invalid { false };

        std::unique_ptr<gfx::context> m_ctx;
        sdl::event_handler            m_event;

        bool                      m_erase { false };
        sim::cursor               m_cursor;
        std::optional<sdl::point> m_previous_cursor_pos;

        sim::system m_system;

        gfx::renderer m_renderer;


        std::array<bool, 6> m_mouse_button_down;


        void mf_on_frame(gfx::context &ctx);

        auto mf_on_mouse_motion(const sdl::event &event) -> sdl::event_return;
        auto mf_on_mouse_wheel(const sdl::event &event) -> sdl::event_return;
        auto mf_on_mouse_button_changed(const sdl::event &event)
            -> sdl::event_return;

        auto mf_on_key_down(const sdl::event &event) -> sdl::event_return;
    };
}
