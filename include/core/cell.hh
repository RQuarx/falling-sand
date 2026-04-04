#pragma once
#include "sdl/color.hh"
#include "sdl/typedefs.hh"


namespace kei::core
{
    struct cell
    {
        friend class grid;

        int        element;
        sdl::color color;

        sdl::fpoint velocity;
        float       temperature;
        int         life;


        constexpr cell(int element = -1) noexcept : element { element } {}
    };
}
