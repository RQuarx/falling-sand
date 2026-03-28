#pragma once

#include <SDL3/SDL_rect.h>


namespace kei::sdl
{
    using point  = SDL_Point;
    using fpoint = SDL_FPoint;

    using rect  = SDL_Rect;
    using frect = SDL_FRect;

    struct size
    {
        int w;
        int h;
    };


    struct fsize
    {
        float w;
        float h;
    };


    [[nodiscard]]
    constexpr auto
    rect_to_frect(rect r) noexcept -> frect
    {
        return { static_cast<float>(r.x), static_cast<float>(r.y),
                 static_cast<float>(r.w), static_cast<float>(r.h) };
    }
}


[[nodiscard]]
constexpr auto
operator==(const kei::sdl::point &a, const kei::sdl::point &b) noexcept -> bool
{
    return a.x == b.x && a.y == b.y;
}

[[nodiscard]]
constexpr auto
operator==(const kei::sdl::fpoint &a, const kei::sdl::fpoint &b) noexcept
    -> bool
{
    return a.x == b.x && a.y == b.y;
}

[[nodiscard]]
constexpr auto
operator==(const kei::sdl::size &a, const kei::sdl::size &b) noexcept -> bool
{
    return a.w == b.w && a.h == b.h;
}

[[nodiscard]]
constexpr auto
operator==(const kei::sdl::fsize &a, const kei::sdl::fsize &b) noexcept -> bool
{
    return a.w == b.w && a.h == b.h;
}

[[nodiscard]]
constexpr auto
operator==(const kei::sdl::rect &a, const kei::sdl::rect &b) noexcept -> bool
{
    return a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h;
}

[[nodiscard]]
constexpr auto
operator==(const kei::sdl::frect &a, const kei::sdl::frect &b) noexcept -> bool
{
    return a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h;
}
