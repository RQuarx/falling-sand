#pragma once
#include "../element.hh"


namespace kei::sim::elements
{
    [[nodiscard]]
    constexpr auto
    air() -> element_def
    {
        return element_def {
            .name               = "air",
            .color              = 0x000000_rgb,
            .color_random_range = 0,
            .mass               = 0,
        };
    }
}
