#pragma once
#include <expected>

#include <SDL3/SDL_video.h>

#include "sdl/resource.hh"
#include "sdl/typedefs.hh"


namespace kei::sdl
{
    struct window final : public resource<SDL_Window, SDL_DestroyWindow>
    {
        [[nodiscard]] auto get_size() const noexcept -> std::expected<sdl::size, class error>;
        [[nodiscard]]
        auto get_size_in_pixels() const noexcept -> std::expected<sdl::size, class error>;
    };
}
