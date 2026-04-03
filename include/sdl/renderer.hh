#pragma once
#include <SDL3/SDL_render.h>

#include "sdl/color.hh"
#include "sdl/resource.hh"
#include "sdl/typedefs.hh"


namespace kei::sdl
{
    struct renderer final : public resource<SDL_Renderer, SDL_DestroyRenderer>
    {
        void clear();
        void present();

        void set_draw_color(sdl::color color);

        void render_texture(resource<SDL_Texture, SDL_DestroyTexture> &texture,
                            const sdl::frect                          *srcrect,
                            const sdl::frect                          *dstrect);


        [[nodiscard]]
        auto create_texture(SDL_PixelFormat pixel_fmt, SDL_TextureAccess access, sdl::size size)
            -> SDL_Texture *;

        [[nodiscard]] auto render_position_from_window(sdl::fpoint window_point) -> sdl::fpoint;
    };
}
