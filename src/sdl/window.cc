#include "error.hh"
#include "sdl/error.hh"
#include "sdl/window.hh"

using kei::sdl::window;


auto
window::get_size() const -> sdl::size
{
    sdl::size size;

    if (!SDL_GetWindowSize(get(), &size.w, &size.h)) throw sdl::error_builder["sdl::window"]();
    return size;
}


auto
window::get_size_in_pixels() const -> sdl::size
{
    sdl::size size;

    if (!SDL_GetWindowSizeInPixels(get(), &size.w, &size.h))
        throw sdl::error_builder["sdl::window"]();
    return size;
}
