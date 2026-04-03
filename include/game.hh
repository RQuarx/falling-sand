#pragma once
#include "core/simulation.hh"
#include "graphics/context.hh"
#include "graphics/renderer.hh"
#include "input/context.hh"
#include "ui/cursor.hh"


namespace kei
{
    class game
    {
    public:
        game() noexcept;
        ~game();


        [[nodiscard]] auto run() noexcept -> int;

    private:
        bool m_valid;

        std::unique_ptr<gfx::context> m_gfx_ctx;
        input::context                m_input_ctx;

        sdl::event_handler m_event_handler;

        gfx::renderer m_renderer;

        core::simulation m_simulation;
        ui::cursor       m_cursor;

        int m_draw_element;


        void mf_on_frame(gfx::context &ctx);
    };
}
