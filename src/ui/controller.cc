#include "core/elements.hh"
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
    using input::button_state;

    auto left_or_right_button {
        input_context.is_button_states()[SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT]
    };
    auto right_button { input_context.is_button_states()[SDL_BUTTON_RIGHT] };
    auto middle_button { input_context.is_button_states()[SDL_BUTTON_MIDDLE] };

    if (middle_button(button_state::pressed))
    {
        sdl::point grid_position { m_cursor.mouse_position_to_grid_position(
            gfx_context.renderer(), input_context.get_cursor_current_position()) };
        sim.set_draw_element(sim.get_grid()[grid_position].element);

        return;
    }

    int prev_draw_element { sim.get_draw_element() };
    if (right_button(button_state::pressed, button_state::down))
        sim.set_draw_element(core::elements::air.id);

    if (left_or_right_button(button_state::pressed))
        sim.draw(m_cursor.get_points_to_draw(input_context.get_cursor_current_position(),
                                             gfx_context.renderer()));
    else if (left_or_right_button(button_state::down))
        sim.draw(m_cursor.get_points_to_draw(input_context.get_cursor_previous_position(),
                                             input_context.get_cursor_current_position(),
                                             gfx_context.renderer()));

    sim.set_draw_element(prev_draw_element);

    renderer.render_cursor_to_texture(gfx_context, sim.get_grid(), m_cursor,
                                      input_context.get_cursor_current_position());
}
