#pragma once
#include <string_view>
#include <vector>

#include "sdl/color.hh"


namespace kei::sim
{
    inline float G { 10'000 }; /* In cell/frame */


    using element_id = std::size_t;

    struct element_def
    {
        element_id       id { 0 };
        std::string_view name;

        sdl::color color;
        int        color_random_range;

        float mass;
        float friction;
    };


    class element_registry
    {
    public:
        [[nodiscard]]
        auto operator[](element_id id) const -> const element_def &;

        [[nodiscard]]
        auto operator<<(element_def elem) -> element_id;

    private:
        std::vector<element_def> m_elements;
    };
}
