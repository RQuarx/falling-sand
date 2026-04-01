#pragma once
#include <SDL3/SDL_error.h>

#include "../error.hh"


namespace kei::sdl
{
    namespace _impl
    {
        auto sdl_error_formatter { [](std::string msg) -> std::string
                                   {
                                       if (msg.empty()) return SDL_GetError();

                                       return std::format("{}: {}", msg, SDL_GetError());
                                   } };
    }


    inline constexpr kei::_impl::error_builder<decltype(_impl::sdl_error_formatter)>
        error_builder {};
}
