#pragma once
#include <cstdint>

#include <SDL3/SDL_events.h>

#include "signal/signal.hh"


namespace kei::sdl
{
    using event      = SDL_Event;
    using event_type = SDL_EventType;


    enum class event_return : std::uint8_t
    {
        exit_fail,
        exit_success,
        success
    };


    class event_handler
    {
        using signal_type = sig::signal<event_return, const sdl::event &>;

    public:
        event_handler() = default;


        [[nodiscard]]
        auto operator[](sdl::event_type type) noexcept
            -> sig::signal_connect<signal_type>;


        auto poll() -> event_return;

    private:
        std::unordered_map<sdl::event_type, signal_type> m_callbacks;
    };
}
