#include "sdl/error.hh"
#include "sdl/renderer.hh"

using kei::sdl::renderer;


void
renderer::clear()
{
    if (!SDL_RenderClear(get())) throw sdl::error_builder["sdl::renderer"]();
}


void
renderer::present()
{
    if (!SDL_RenderPresent(get())) throw sdl::error_builder["sdl::renderer"]();
}

void
renderer::set_draw_color(sdl::color color)
{
    if (!SDL_SetRenderDrawColor(get(), color.r, color.g, color.b, color.a))
        throw sdl::error_builder["sdl::renderer"]();
}

void
renderer::render_texture(resource<SDL_Texture, SDL_DestroyTexture> &texture,
                         const sdl::frect                          *srcrect,
                         const sdl::frect                          *dstrect)
{
    if (!SDL_RenderTexture(get(), texture.get(), srcrect, dstrect))
        throw sdl::error_builder["sdl::renderer"]();
}


auto
renderer::create_texture(SDL_PixelFormat pixel_fmt, SDL_TextureAccess access, sdl::size size)
    -> SDL_Texture *
{
    if (auto *t { SDL_CreateTexture(get(), pixel_fmt, access, size.w, size.h) }; t == nullptr)
        throw sdl::error_builder["sdl::renderer"]();
    else /* NOLINT */
        return t;
}


auto
renderer::render_position_from_window(sdl::fpoint window_point) -> sdl::fpoint
{
    sdl::fpoint point;
    if (!SDL_RenderCoordinatesFromWindow(get(), window_point.x, window_point.y, &point.x, &point.y))
        throw sdl::error_builder["sdl::renderer"]();
    return point;
}
