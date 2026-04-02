#include "sdl/texture.hh"

using kei::sdl::texture;


auto
texture::size() const -> sdl::fsize
{
    sdl::fsize size;
    if (!SDL_GetTextureSize(get(), &size.w, &size.h)) throw sdl::error_builder["sdl::texture"]();
    return size;
}


void
texture::set_scale_mode(SDL_ScaleMode mode)
{
    if (!SDL_SetTextureScaleMode(get(), mode)) throw sdl::error_builder["sdl::texture"]();
}


auto
texture::lock(const sdl::rect *rect) -> texture_pixel
{ return texture_pixel { *this, rect }; }
