#pragma once
#include <cstddef>

#include <SDL3/SDL_render.h>

#include "error.hh"
#include "sdl/color.hh"
#include "sdl/resource.hh"
#include "sdl/typedefs.hh"


namespace kei::sdl
{
    struct texture final : public resource<SDL_Texture, SDL_DestroyTexture>
    {
        [[nodiscard]] auto size() const -> sdl::fsize;
        [[nodiscard]] auto lock(const sdl::rect *rect) -> class texture_pixel;

        void set_scale_mode(SDL_ScaleMode mode);
    };


    class texture_pixel
    {
    public:
        texture_pixel(texture &texture, const sdl::rect *rect) : m_texture { texture }
        {
            if (!SDL_LockTexture(m_texture.get(), rect, &m_pixels, &m_pitch))
                throw sdl::error_builder["sdl::texture"]();
        }

        ~texture_pixel() { SDL_UnlockTexture(m_texture.get()); }


        void
        set_color_at(sdl::point point, sdl::color color)
        { (*this)[point.y][point.x] = color.to_rgba_uint(); }


    private:
        texture &m_texture;

        void *m_pixels { nullptr };
        int   m_pitch { 0 };


        [[nodiscard]]
        auto
        operator[](std::size_t y) -> std::uint32_t *
        { return reinterpret_cast<std::uint32_t *>(static_cast<char *>(m_pixels) + (y * m_pitch)); }
    };
}
