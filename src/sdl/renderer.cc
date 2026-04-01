#include "sdl/renderer.hh"

using kei::sdl::renderer;


auto
renderer::clear() noexcept -> std::optional<error>
{
    if (!SDL_RenderClear(get())) return sdl::error_builder["sdl::renderer"]();
    return std::nullopt;
}


auto
renderer::present() noexcept -> std::optional<error>
{
    if (!SDL_RenderPresent(get())) return sdl::error_builder["sdl::renderer"]();
    return std::nullopt;
}

auto
renderer::set_draw_color(sdl::color color) noexcept -> std::optional<error>
{
    if (!SDL_SetRenderDrawColor(get(), color.r, color.g, color.b, color.a))
        return sdl::error_builder["sdl::renderer"]();
    return std::nullopt;
}

auto
renderer::render_texture(resource<SDL_Texture, SDL_DestroyTexture> &texture,
                         const sdl::frect                          *srcrect,
                         const sdl::frect *dstrect) noexcept -> std::optional<error>
{
    if (!SDL_RenderTexture(get(), texture.get(), srcrect, dstrect))
        return sdl::error_builder["sdl::renderer"]();
    return std::nullopt;
}


auto
renderer::create_texture(SDL_PixelFormat   pixel_fmt,
                         SDL_TextureAccess access,
                         sdl::fsize        size) noexcept -> std::expected<SDL_Texture *, error>
{
    if (auto *t { SDL_CreateTexture(get(), pixel_fmt, access, size.w, size.h) }; t == nullptr)
        return std::unexpected { sdl::error_builder["sdl::renderer"]() };
    else /* NOLINT */
        return t;
}
