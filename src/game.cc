#include <memory>

#include <SDL3/SDL.h>

#include "error.hh"
#include "game.hh"
#include "logger.hh"
#include "signal/bind.hh"
#include "signal/method.hh"
#include "views/bresenham.hh"

using kei::game;


namespace kei
{
    static auto
    get_log_level_threshold() noexcept -> log_level
    {
        using namespace std::string_view_literals;

        const char *threshold { std::getenv("LOGLEVEL_THRESHOLD") };
        if (threshold == nullptr) return log_level::warn;

        if (threshold == "trace"sv) return log_level::trace;
        if (threshold == "debug"sv) return log_level::debug;
        if (threshold == "info"sv) return log_level::info;
        if (threshold == "warn"sv) return log_level::warn;
        if (threshold == "error"sv) return log_level::error;
        if (threshold == "fatal"sv) return log_level::fatal;

        return log_level::warn;
    }


    class logger logger { get_log_level_threshold() };
}


game::game()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        logger[log_level::error, "game"]("Failed to initialize SDL: {}",
                                         SDL_GetError());
        m_invalid = true;
        return;
    }

    m_ctx = std::make_unique<gfx::context>("Kei's Falling Sand Game", 900, 600,
                                           SDL_WINDOW_HIGH_PIXEL_DENSITY
                                               | SDL_WINDOW_RESIZABLE
                                               | SDL_WINDOW_VULKAN);
    m_ctx->signal_on_frame() | sig::method(*this, &game::mf_on_frame);

    m_event[SDL_EVENT_MOUSE_MOTION]
        | sig::method(*this, &game::mf_on_mouse_motion);
    m_event[SDL_EVENT_MOUSE_BUTTON_DOWN]
        | sig::method(*this, &game::mf_on_mouse_button_changed);
    m_event[SDL_EVENT_MOUSE_BUTTON_UP]
        | sig::method(*this, &game::mf_on_mouse_button_changed);
    m_event[SDL_EVENT_MOUSE_WHEEL]
        | sig::method(*this, &game::mf_on_mouse_wheel);
    m_event[SDL_EVENT_KEY_DOWN] | sig::method(*this, &game::mf_on_key_down);


    m_event[SDL_EVENT_QUIT] | [](const sdl::event &) -> sdl::event_return
    { return sdl::event_return::exit_success; };

    m_cursor.shape   = sim::cursor::shape::circle;
    m_cursor.size    = { .w = 20, .h = 20 };
    m_cursor.draw_id = sim::element_registry->sand;
}


game::~game()
{
    if (!m_invalid) SDL_Quit();
}


auto
game::run() noexcept -> int
{
    if (m_invalid) return 1;

    std::uint64_t last_frame_time { SDL_GetTicks() };
    while (true) try
        {
            auto  current_time { SDL_GetTicks() };
            float dt { (current_time - last_frame_time) / 1000.F };
            last_frame_time = current_time;

            switch (m_event.poll())
            {
            case sdl::event_return::exit_fail:    return 1;
            case sdl::event_return::exit_success: return 2;
            case sdl::event_return::success:      break;
            }


            if (m_mouse_button_down[SDL_BUTTON_LEFT]
                || m_mouse_button_down[SDL_BUTTON_RIGHT])
            {
                if (m_previous_cursor_pos)
                    m_system.enqueue_draw(
                        sig::bind(sig::method(*this, &game::mf_draw_line),
                                  sig::bind_to::position_1 {},
                                  sig::bind_to::position_2 {}));
                else
                    m_system.enqueue_draw(
                        sig::bind(sig::method(m_cursor, &sim::cursor::draw),
                                  sig::bind_to::position_1 {},
                                  sig::bind_to::position_2 {}, m_erase));

                m_previous_cursor_pos = m_cursor.pos;
            }

            m_system.update(dt);

            m_ctx->do_frame();
        }
        catch (class error err)
        {
            logger[log_level::error, "game", err.get_source()](
                "{}", err.get_message());
            return 1;
        }
    return 0;
}


void
game::mf_draw_line(sdl::size                       board_size,
                   std::vector<sim::draw_command> &cells_to_draw)
{
    for (auto pos : views::bresenham(*m_previous_cursor_pos, m_cursor.pos))
    {
        m_cursor.pos = pos;
        m_cursor.draw(board_size, cells_to_draw, m_erase);
    }
}


void
game::mf_on_frame(gfx::context &ctx)
{
    if (auto e { m_renderer.render(ctx, m_system.board().size(),
                                   m_system.get_present_cells(), m_cursor) })
        logger[log_level::error, e->get_domain(), e->get_source()](
            "{}", e->get_message());
}


auto
game::mf_on_mouse_motion(const sdl::event &event) -> sdl::event_return
{
    sdl::fpoint mouse;

    SDL_RenderCoordinatesFromWindow(m_ctx->get_render().get(), event.motion.x,
                                    event.motion.y, &mouse.x, &mouse.y);

    sdl::frect board_rect { m_renderer.get_board_rect() };
    float      cell_size { m_renderer.get_cell_size() };

    sdl::fpoint relative_pos {
        .x = std::clamp(mouse.x - board_rect.x, 0.F, board_rect.w - 1),
        .y = std::clamp(mouse.y - board_rect.y, 0.F, board_rect.h - 1)
    };

    m_cursor.pos = sdl::point {
        .x = static_cast<int>(relative_pos.x / cell_size),
        .y = static_cast<int>(relative_pos.y / cell_size),
    };


    return sdl::event_return::success;
}


auto
game::mf_on_mouse_button_changed(const sdl::event &event) -> sdl::event_return
{
    if (event.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:  m_erase = false; break;
        case SDL_BUTTON_RIGHT: m_erase = true; break;
        case SDL_BUTTON_MIDDLE:
            m_cursor.draw_id = m_system.board()[m_cursor.pos].element;
            break;
        default: break;
        }
    }

    m_mouse_button_down[event.button.button]
        = event.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN;

    if (m_mouse_button_down[SDL_BUTTON_LEFT]
        || m_mouse_button_down[SDL_BUTTON_RIGHT])
        m_previous_cursor_pos = m_cursor.pos;
    else
        m_previous_cursor_pos = std::nullopt;

    return sdl::event_return::success;
}


auto
game::mf_on_mouse_wheel(const sdl::event &event) -> sdl::event_return
{
    int delta { static_cast<int>(std::round(event.wheel.y)) };

    if ((SDL_GetModState() & SDL_KMOD_SHIFT) != 0U)
        m_cursor.size.w = std::max(1, m_cursor.size.w + delta);
    else if ((SDL_GetModState() & SDL_KMOD_CTRL) != 0U)
        m_cursor.size.h = std::max(1, m_cursor.size.h + delta);
    else
    {
        m_cursor.size.w = std::max(1, m_cursor.size.w + delta);
        m_cursor.size.h = std::max(1, m_cursor.size.h + delta);
    }

    return sdl::event_return::success;
}


auto
game::mf_on_key_down(const sdl::event &event) -> sdl::event_return
{
    if (event.key.scancode == SDL_SCANCODE_TAB)
    {
        switch (m_cursor.shape)
        {
        case sim::cursor::shape::square:
            m_cursor.shape = sim::cursor::shape::triangle;
            break;
        case sim::cursor::shape::circle:
            m_cursor.shape = sim::cursor::shape::square;
            break;
        case sim::cursor::shape::triangle:
            m_cursor.shape = sim::cursor::shape::circle;
            break;
        }
    }

    return sdl::event_return::success;
}
