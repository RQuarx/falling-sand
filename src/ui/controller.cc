#include "ui/controller.hh"

using kei::ui::controller;


void
controller::connect_signals(sdl::event_handler &event_handler, gfx::renderer &renderer)
{ m_cursor.connect_signals(event_handler, renderer); }


void
controller::on_frame(core::simulation &sim,
                     gfx::renderer    &renderer,
                     gfx::context     &gfx_context,
                     input::context   &input_context)
{
    if (input_context.is_button_states()[SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT](
            input::button_state::pressed))
        sim.draw(m_cursor.get_points_to_draw(input_context.get_cursor_current_position(),
         gfx_context.renderer()));
        // sim.draw(m_cursor.get_points_to_draw(input_context.get_cursor_previous_position(),
        //                                      input_context.get_cursor_current_position(),
        //                                      gfx_context.renderer()));
    else if (input_context.is_button_states()[SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT](
                 input::button_state::down))
        sim.draw(m_cursor.get_points_to_draw(input_context.get_cursor_previous_position(),
                                             input_context.get_cursor_current_position(),
                                             gfx_context.renderer()));

    renderer.render_cursor_to_texture(gfx_context, sim.get_grid(), m_cursor,
                                      input_context.get_cursor_current_position());
}
