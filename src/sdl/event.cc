#include "sdl/event.hh"

using kei::sdl::event_handler;


auto
event_handler::operator[](sdl::event_type type) noexcept
    -> sig::signal_connect<signal_type>
{
    return sig::signal_connect { m_callbacks[type] };
}


auto
event_handler::poll() -> event_return
{
    sdl::event ev;

    while (SDL_PollEvent(&ev))
        if (auto it { m_callbacks.find(sdl::event_type(ev.type)) };
            it != m_callbacks.end())
        {
            auto retvals { it->second.emit(ev) };
            auto find_it { std::ranges::find_if_not(
                retvals,
                [](event_return v) { return v == event_return::success; }) };

            if (find_it != retvals.end()) return *find_it;
        }

    return event_return::success;
}
