#include <utility>

#include "input/context.hh"
#include "sdl/renderer.hh"
#include "signal/bind.hh"
#include "signal/method.hh"

using kei::input::context;


void
context::connect_signals(sdl::event_handler &event_handler, sdl::renderer &renderer)
{
    auto method { sig::bind(sig::method(*this, &context::update), sig::bind_to::position_1 {},
                            std::ref(renderer)) };

    event_handler[SDL_EVENT_MOUSE_MOTION] | method;
    event_handler[SDL_EVENT_MOUSE_BUTTON_DOWN] | method;
    event_handler[SDL_EVENT_MOUSE_BUTTON_UP] | method;
    event_handler[SDL_EVENT_MOUSE_WHEEL] | method;
    event_handler[SDL_EVENT_KEY_DOWN] | method;
    event_handler[SDL_EVENT_KEY_UP] | method;
    event_handler[SDL_EVENT_TEXT_INPUT] | method;
}


void
context::on_frame_begin()
{
    m_cursor.current_position  = {};
    m_cursor.previous_position = {};

    m_mouse_wheel = {};

    m_text_input.clear();

    m_keys_down |= m_keys_pressed;
    m_keys_pressed.reset();

    m_keys_up |= m_keys_released;
    m_keys_released.reset();

    for (button_state &state : m_mouse_buttons) switch (state)
        {
        case button_state::pressed:  state = button_state::down; break;
        case button_state::released: state = button_state::up; break;
        case button_state::down:     [[fallthrough]];
        case button_state::up:       break;
        }
}


auto
context::update(const sdl::event &ev, sdl::renderer &renderer) -> sdl::event_return
{
    switch (ev.type)
    {
    case SDL_EVENT_MOUSE_MOTION:
        m_cursor.previous_position = std::exchange(
            m_cursor.current_position,
            renderer.render_position_from_window({ .x = ev.motion.x, .y = ev.motion.y }));
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        {
            float x { ev.wheel.x };
            float y { ev.wheel.y };

            if (ev.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
            {
                x = -x;
                y = -y;
            }

            m_mouse_wheel.x += x;
            m_mouse_wheel.y += y;
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        m_mouse_buttons[ev.button.button] = button_state::pressed;
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        m_mouse_buttons[ev.button.button] = button_state::released;
        break;

    case SDL_EVENT_KEY_DOWN:
        {
            auto scancode { ev.key.scancode };

            m_keys_pressed.set(scancode);

            m_keys_up.reset(scancode);
            m_keys_down.reset(scancode);
            m_keys_released.reset(scancode);
        }
        break;

    case SDL_EVENT_KEY_UP:
        {
            auto scancode { ev.key.scancode };

            m_keys_released.set(scancode);

            m_keys_up.reset(scancode);
            m_keys_down.reset(scancode);
            m_keys_pressed.reset(scancode);
        }
        break;

    case SDL_EVENT_TEXT_INPUT: m_text_input += ev.text.text; break;
    default:                   break;
    }

    return sdl::event_return::success;
}


auto
context::get_cursor_current_position() const noexcept -> sdl::fpoint
{ return m_cursor.current_position; }


auto
context::get_cursor_previous_position() const noexcept -> sdl::fpoint
{ return m_cursor.previous_position; }


auto
context::get_mouse_wheel_direction() const noexcept -> sdl::fpoint
{ return m_mouse_wheel; }


auto
context::get_mouse_button_state(int button_index) const noexcept -> button_state
{ return m_mouse_buttons[button_index]; }


auto
context::get_key_state(SDL_Scancode scancode) const noexcept -> button_state
{
    if (m_keys_up.test(scancode)) return button_state::up;
    if (m_keys_pressed.test(scancode)) return button_state::pressed;
    if (m_keys_released.test(scancode)) return button_state::released;
    if (m_keys_down.test(scancode)) return button_state::down;
    return button_state::up;
}


auto
context::get_text_input() const noexcept -> const std::string &
{ return m_text_input; }


auto
context::is_button_states() const noexcept
    -> is_x_states<decltype(&context::get_mouse_button_state)>
{ return { *this, &context::get_mouse_button_state }; }


auto
context::is_key_states() const noexcept
    -> is_x_states<decltype(&context::get_key_state)>
{ return { *this, &context::get_key_state }; }
