#include <SDL3/SDL.h>

#include "game.hh"
#include "logger.hh"

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


game::game() noexcept : m_valid { true }
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        logger[log_level::fatal, "game"]("Failed to initialize SDL: {}", SDL_GetError());
        m_valid = false;
        return;
    }

    m_gfx_ctx = std::make_unique<gfx::context>(
        "Kei's Falling Sand Game", 900, 600, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);

    m_cursor.connect_signals(m_event_handler, m_renderer);
    m_input_ctx.connect_signals(m_event_handler, m_gfx_ctx->render());
}


auto
game::run() noexcept -> int
{
    using input::button_state;

    if (!m_valid) return 1;

    std::uint64_t last_frame_time { SDL_GetTicks() };

    while (true) try
        {
            auto  current_time { SDL_GetTicks() };
            float dt { (current_time - last_frame_time) / 1000.F };
            last_frame_time = current_time;

            switch (m_event_handler.poll())
            {
            case sdl::event_return::exit_fail:    return 1;
            case sdl::event_return::exit_success: return 2;
            case sdl::event_return::success:      break;
            }

            if (m_input_ctx.is_button_states()[SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT](
                    button_state::down, button_state::pressed))
            {
                const auto &points { m_cursor.get_points_to_draw(m_gfx_ctx->render()) };
                m_simulation.draw(points, m_draw_element);
            }

            m_gfx_ctx->do_frame(0x000000_rgb);
        }
        catch (class error err)
        {
            logger[log_level::error, err.get_domain(), err.get_source()]("{}", err.get_message());
            return 1;
        }
    return 0;
}
