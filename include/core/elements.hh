#pragma once
#include "core/element_definition.hh"


namespace kei::core
{
    namespace elements
    {
        constexpr inline element_definition air {
            .name = "air", .id = -1, .color = 0x00000000_rgba, .drag = 1
        };
    }


    [[nodiscard]]
    constexpr auto
    get_element_definition(int id) -> const element_definition &
    {
        switch (id)
        {
        default: return elements::air;
        }
    }
}
