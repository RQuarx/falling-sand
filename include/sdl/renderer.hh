#pragma once
#include <SDL3/SDL_render.h>

#include "error.hh"
#include "sdl/color.hh"
#include "sdl/resource.hh"
#include "sdl/typedefs.hh"


namespace kei::sdl
{
    struct renderer final : public resource<SDL_Renderer, SDL_DestroyRenderer>
    {
        auto clear() noexcept -> std::optional<error>;
        auto present() noexcept -> std::optional<error>;

        auto set_draw_color(sdl::color color) noexcept -> std::optional<error>;

        auto render_texture(resource<SDL_Texture, SDL_DestroyTexture> &texture,
                            const sdl::frect                          *srcrect,
                            const sdl::frect *dstrect) noexcept
            -> std::optional<error>;
    };
}
