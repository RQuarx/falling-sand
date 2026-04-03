#pragma once
#include <cmath>
#include <cstdint>
#include <format>
#include <string_view>

#include <SDL3/SDL_pixels.h>


namespace kei::sdl
{
    struct alignas(alignof(std::uint32_t)) color
    {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a { 255 };


        constexpr color() noexcept = default;
        constexpr color(std::uint8_t r,
                        std::uint8_t g,
                        std::uint8_t b,
                        std::uint8_t a = 255) noexcept
            : r(r), g(g), b(b), a(a)
        {
        }


        static constexpr auto
        from_rgb(std::uint32_t value) noexcept -> color
        {
            return color { static_cast<std::uint8_t>((value >> 16) & 0xFF),
                           static_cast<std::uint8_t>((value >> 8) & 0xFF),
                           static_cast<std::uint8_t>((value) & 0xFF), 255 };
        }


        static constexpr auto
        from_rgba(std::uint32_t value) noexcept -> color
        {
            return color {
                static_cast<std::uint8_t>((value >> 24) & 0xFF),
                static_cast<std::uint8_t>((value >> 16) & 0xFF),
                static_cast<std::uint8_t>((value >> 8) & 0xFF),
                static_cast<std::uint8_t>((value) & 0xFF),
            };
        }


        [[nodiscard]]
        constexpr auto
        to_rgb_uint() const noexcept -> std::uint32_t
        { return (r << 16) | (g << 8) | b; }


        [[nodiscard]]
        constexpr auto
        to_rgba_uint() const noexcept -> std::uint32_t
        { return (r << 24) | (g << 16) | (b << 8) | a; }


        [[nodiscard]]
        constexpr auto
        to_color() const noexcept -> SDL_Color
        { return { .r = r, .g = g, .b = b, .a = a }; }


        [[nodiscard]]
        constexpr auto
        to_fcolor() const noexcept -> SDL_FColor
        { return { .r = r / 255.0F, .g = g / 255.0F, .b = b / 255.0F, .a = a / 255.0F }; }


        constexpr auto
        operator==(const sdl::color &other) const noexcept -> bool
        { return r == other.r && g == other.g && b == other.b && a == other.a; }


        constexpr
        operator SDL_Color() const noexcept
        { return to_color(); }


        constexpr
        operator SDL_FColor() const noexcept
        { return to_fcolor(); }


        [[nodiscard]]
        constexpr auto
        lerp(color other, float t) const noexcept -> color
        {
            auto lerp8 { [](std::uint8_t a, std::uint8_t b, float t) -> std::uint8_t
                         {
                             return static_cast<std::uint8_t>(
                                 std::lerp(static_cast<float>(a), static_cast<float>(b), t));
                         } };

            return {
                lerp8(r, other.r, t),
                lerp8(g, other.g, t),
                lerp8(b, other.b, t),
                lerp8(a, other.a, t),
            };
        }


        static constexpr auto
        hex_val(char c) noexcept -> int
        {
            return (c >= '0' && c <= '9') ? c - '0'
                 : (c >= 'A' && c <= 'F') ? c - 'A' + 10
                 : (c >= 'a' && c <= 'f') ? c - 'a' + 10
                                          : -1;
        }


        static constexpr auto
        from_hex(std::string_view hex) -> color
        {
            auto read_byte { [hex](int pos) -> std::uint8_t
                             {
                                 int hi { hex_val(hex[pos]) };
                                 int lo { hex_val(hex[pos + 1]) };

                                 if (hi < 0 || lo < 0) return 0;

                                 return static_cast<std::uint8_t>((hi * 16) + lo);
                             } };

            if (hex.length() == 6) return color { read_byte(0), read_byte(2), read_byte(4), 255 };
            return color { read_byte(0), read_byte(2), read_byte(4), read_byte(6) };
        }
    };


    inline constexpr color white { 255, 255, 255 };
    inline constexpr color black { 0, 0, 0 };
}


static constexpr auto
operator""_rgb(const unsigned long long color) -> kei::sdl::color
{ return kei::sdl::color::from_rgb(color); }


static constexpr auto
operator""_rgba(const unsigned long long color) -> kei::sdl::color
{ return kei::sdl::color::from_rgba(color); }


static constexpr auto
operator""_rgb(const char *hex, std::size_t len) -> kei::sdl::color
{ return kei::sdl::color::from_hex({ hex, len }); }


static constexpr auto
operator""_rgba(const char *hex, std::size_t len) -> kei::sdl::color
{ return kei::sdl::color::from_hex({ hex, len }); }


/* clang-format off */
template <>
struct std::formatter<kei::sdl::color>
{
public:
    template <class ParserContext>
    constexpr auto
    parse(ParserContext &ctx) -> ParserContext::iterator
    {
        auto it { ctx.begin() };

        if (it == ctx.end()) return it;

        if (it != ctx.end() && *it != '}')
            throw std::format_error {
                "invalid format args for kei::sdl::color"
            };
        return it;
    }


    template <class FormatContext>
    auto
    format(kei::sdl::color color, FormatContext &ctx) const
    {
        return std::format_to(ctx.out(),
                              m_hex ? "#{:02x}{:02x}{:02x}{:02x}"
                                    : "rgba({}, {}, {}, {})",
                              color.r, color.g, color.b, color.a);
    }

private:
    bool m_hex;


    template <class ParserContext>
    [[nodiscard]]
    static auto
    is_char(ParserContext &ctx, char c) -> bool
    {
        bool val { ctx != ctx.end() ? *ctx == c : false };
        return val ? ctx++, val : false;
    }


    template <class ParserContext>
    void
    mf_is_hex(ParserContext &ctx)
    {
        if (is_char(ctx, 'h') && is_char(ctx, 'e') && is_char(ctx, 'x'))
            m_hex = true;
    }
};


template <>
struct std::hash<kei::sdl::color>
{
    auto
    operator()(const kei::sdl::color &c) const noexcept -> std::size_t
    {
        return std::hash<std::uint32_t> {}(c.to_rgba_uint());
    }
};
/* clang-format on */
