#pragma once
#include "../element.hh"


namespace kei::sim::elements
{
    [[nodiscard]]
    constexpr auto
    sand() -> element_def
    {
        return element_def {
            .name               = "sand",
            .color              = 0xf6d7b0_rgb,
            .color_random_range = 10,
            .mass               = 1,
            .friction           = 0.7,
        };
    }
}
