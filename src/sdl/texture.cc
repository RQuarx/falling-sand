#include "sdl/texture.hh"

using kei::sdl::texture;


auto
texture::size() const noexcept -> std::expected<sdl::fsize, error>
{
    sdl::fsize size;
    if (!SDL_GetTextureSize(get(), &size.w, &size.h))
        return std::unexpected { sdl::error_builder["sdl::texture"]() };
    return size;
}


auto
texture::set_scale_mode(SDL_ScaleMode mode) noexcept -> std::optional<error>
{
    if (!SDL_SetTextureScaleMode(get(), mode)) return sdl::error_builder["sdl::texture"]();
    return std::nullopt;
}


auto
texture::lock(const sdl::rect *rect) -> std::expected<class texture_pixel, error>
{
    try
    {
        return texture_pixel { *this, rect };
    }
    catch (const error &e)
    {
        return std::unexpected { e };
    }
}
