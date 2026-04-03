#pragma once
#include "core/simulation.hh"
#include "cursor.hh"
#include "input/context.hh"


namespace kei::ui
{
    class controller
    {
    public:
        void connect_signals(sdl::event_handler &event_handler, gfx::renderer &renderer);


        void on_frame(core::simulation &sim,
                      gfx::renderer    &renderer,
                      gfx::context     &gfx_context,
                      input::context   &input_context);

    private:
        class cursor m_cursor;
    };
}
