#pragma once
#include "core/element_definition.hh"


namespace kei::core
{
    namespace elements
    {
        constexpr inline element_definition air {
            .name = "air", .id = -1, .color = 0x000000_rgb, .drag = 1
        };
        constexpr inline element_definition sand {
            .name = "sand", .id = 1, .color = 0xf6d7b0_rgb, .drag = 1
        };
    }


    [[nodiscard]]
    constexpr auto
    get_element_definition(int id) -> const element_definition &
    {
        switch (id)
        {
        case elements::sand.id: return elements::sand;
        default:                return elements::air;
        }
    }
}
