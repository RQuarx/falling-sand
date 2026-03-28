#pragma once
#include <expected>
#include <optional>

#include <SDL3/SDL_render.h>

#include "error.hh"
#include "sdl/resource.hh"
#include "sdl/typedefs.hh"


namespace kei::sdl
{
    struct texture final : public resource<SDL_Texture, SDL_DestroyTexture>
    {
        [[nodiscard]]
        auto size() const noexcept -> std::expected<sdl::fsize, error>;

        auto set_scale_mode(SDL_ScaleMode mode) noexcept
            -> std::optional<error>;


        [[nodiscard]]
        auto lock(const sdl::rect *rect)
            -> std::expected<class texture_pixel, error>;
    };


    class texture_pixel
    {
    public:
        texture_pixel(texture &texture, const sdl::rect *rect)
            : m_texture { texture }
        {
            if (!SDL_LockTexture(m_texture.get(), rect, &m_pixels, &m_pitch))
                throw sdl::error_builder["sdl::texture"]();
        }

        ~texture_pixel() { SDL_UnlockTexture(m_texture.get()); }


        [[nodiscard]]
        auto
        operator[](std::size_t y) -> std::uint32_t *
        {
            return reinterpret_cast<std::uint32_t *>(
                static_cast<char *>(m_pixels) + (y * m_pitch));
        }


    private:
        texture &m_texture;

        void *m_pixels { nullptr };
        int   m_pitch { 0 };
    };
}
