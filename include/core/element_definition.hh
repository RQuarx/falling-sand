#pragma once
#include <string_view>

#include "sdl/color.hh"
#include "sdl/typedefs.hh"


namespace kei::core
{
    using update_fn = void (*)(class simulation &, sdl::point);


    struct element_definition
    {
        const std::string_view name;
        const int              id {};
        const sdl::color       color;
        const int              random_color_range;


        const float mass {};
        const float friction {};
        const float drag {};


        const update_fn update { nullptr };
    };
}
