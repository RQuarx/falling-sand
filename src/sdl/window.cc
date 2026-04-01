#include "error.hh"
#include "sdl/error.hh"
#include "sdl/window.hh"

using kei::sdl::window;


auto
window::get_size() const noexcept -> std::expected<sdl::size, kei::error>
{
    sdl::size size;

    if (!SDL_GetWindowSize(get(), &size.w, &size.h))
        return std::unexpected { sdl::error_builder["sdl::window"]() };
    return size;
}


auto
window::get_size_in_pixels() const noexcept -> std::expected<sdl::size, kei::error>
{
    sdl::size size;

    if (!SDL_GetWindowSizeInPixels(get(), &size.w, &size.h))
        return std::unexpected { sdl::error_builder["sdl::window"]() };
    return size;
}
